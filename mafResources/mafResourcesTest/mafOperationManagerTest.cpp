/*
 *  mafOperationManagerTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 11/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafCoreRegistration.h>
#include <mafResourcesSingletons.h>
#include <mafEventBusManager.h>
#include <mafVMEManager.h>
#include <mafOperationManager.h>
#include <mafOperation.h>
//#include <mafVME.h>
#include <mafOperationWorker.h>

#define kMAX_COUNT 50000

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

class mafOperationManagerTest;

class testEventLoopTerminator : public QObject {
    Q_OBJECT

public:
    testEventLoopTerminator(mafOperationManagerTest *testClass);

    void setCounter(int c);

public slots:
    void workDoneCounter();
    void startOperation();

signals:
    void terminateLoop();

private:
    mutable QMutex mutex;
    int m_Counter;
    mafOperationManagerTest *m_TestClass;
};


//==========================================================================================
// Objects used in test suite.
//==========================================================================================

/**
 Class name: testEndlessOperation
 This is an example of endless operation that needs to be aborted to tesrminate the test suite.
 */
class testEndlessOperation : public mafResources::mafOperation {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafOperation);

public:
    /// Object constructor.
    testEndlessOperation(const QString code_location = "");

public slots:
    /// execution method
    /*virtual*/ void execute();
};

testEndlessOperation::testEndlessOperation(const QString code_location) : mafOperation(code_location) {
    m_CanUnDo = false;
    setObjectName("testEndlessOperation");
}

void testEndlessOperation::execute() {
    while ( !m_Abort ) {
        ;
    }

    if ( m_Abort ) {
        return;
    }

    emit executionEnded();
}

//------------------------------------------------------------------------------------------------------------------------------------------------

/**
 Class name: testNotUndoOperation
 This operation is an example of not undoable operation.
 */
class testNotUndoOperation : public mafResources::mafOperation {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafOperation);

public:
    /// Object constructor.
    testNotUndoOperation(const QString code_location = "");

    /// Return the internal variable value.
    int val();

public slots:
    /// execution method
    /*virtual*/ void execute();

private:
    int m_Val;
};

testNotUndoOperation::testNotUndoOperation(const QString code_location) : mafOperation(code_location), m_Val(0) {
    m_CanUnDo = false;
    setObjectName("NotUndoOperation");
}

int testNotUndoOperation::val() {
    return m_Val;
}

void testNotUndoOperation::execute() {
    m_Val = 0;
    int i = 0;

    while ( ++i < kMAX_COUNT ) {
        if ( i % 1000 == 0 ) {
            qDebug() << "Current not undo value " << m_Val;
            m_Val += i;
        }
    }

    emit executionEnded();
}

//------------------------------------------------------------------------------------------------------------------------------------------------

/**
 Class name: testUndoOperation
 This operation is an example of undoable operation.
 */
class testUndoOperation : public mafResources::mafOperation {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafOperation);

public:
    /// Object constructor.
    testUndoOperation(const QString code_location = "");

    /// Return the internal variable value.
    int val();

public slots:
    /// execution method
    /*virtual*/ void execute();

    /// Allows to call the piece of algorithm that is needed to restore the previous state of the operation's execution.
    /*virtual*/ void unDo();

    /// Allows to call the piece of algorithm that is needed to apply the operation again.
    /*virtual*/ void reDo();

private:
    int m_Val;
};

testUndoOperation::testUndoOperation(const QString code_location) : mafOperation(code_location), m_Val(0) {
    setObjectName("UndoOperation");
}

int testUndoOperation::val() {
    return m_Val;
}

void testUndoOperation::execute() {
    // Initialize variable
    m_Val = 0;
    int i = 0;

    QString n = this->objectName();
    qDebug() << "Executing " << n << "...";

    // do some heavy calculation...
    while ( ++i < kMAX_COUNT ) {
        if ( i % 1000 == 0 ) {
            qDebug() << n << " current value " << m_Val;
            m_Val += i;
        }
    }

    // set the final result value.
    m_Val = kMAX_COUNT;

    qDebug() << this->objectName() << " emitting executionEnded...";
    emit executionEnded();
}

void testUndoOperation::unDo() {
    m_Val = 0;
}

void testUndoOperation::reDo() {
    m_Val = kMAX_COUNT;
}

//==========================================================================================
// Test Suite
//==========================================================================================

/**
 Class name: mafOperationManagerTest
 This class implements the test suite for mafOperationManager.
 */
class mafOperationManagerTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        m_EventBus = mafEventBusManager::instance();

//        mafResourcesSingletons::mafSingletonsInitialize();
        m_VMEManager = mafVMEManager::instance();
        m_OperationManager = mafOperationManager::instance();

        // Register all the creatable objects for the mafResources module.
        mafResourcesRegistration::registerResourcesObjects();

        // Register custom operations.
        mafRegisterObject(testEndlessOperation);
        mafRegisterObject(testNotUndoOperation);
        mafRegisterObject(testUndoOperation);

        m_LoopManager = new testEventLoopTerminator(this);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        qDebug() << "cleanup test suite...";
        m_OperationManager->shutdown();
        m_VMEManager->shutdown();

        delete m_LoopManager;
        m_LoopManager = NULL;

        // Unregister custom operations.
        mafUnregisterObject(testEndlessOperation);
        mafUnregisterObject(testNotUndoOperation);
        mafUnregisterObject(testUndoOperation);

//        mafResourcesSingletons::mafSingletonsShutdown();

        //restore vme manager status
        m_EventBus->notifyEvent("maf.local.resources.hierarchy.create");

        mafEventBusManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafOperationManager allocation test case.
    void mafOperationManagerAllocationTest();

    /// Abort execution test
    void abortExecutionTest();

    /// Undo-Redo functionality test
    void undoRedoExecutionTest();

public:
    /// Utility method to start operations. Return the operation started.
    const mafCore::mafObjectBase *startOperation(QString opType);

    /// Utility method to retrieve the execution pool.
    const mafExecutionPool *retrievePool();

private:
    mafEventBusManager *m_EventBus; ///< Reference to the event bus.
    mafOperationManager *m_OperationManager; ///< Reference to the OperationManager.
    const mafExecutionPool *m_ExecutionPool; ///< Pool of running thread.
    testEventLoopTerminator *m_LoopManager; ///< Class responsible to terminate the event loop.
    mafVMEManager *m_VMEManager; ///< Manager needed for the hierarchy.
};

const mafCore::mafObjectBase *mafOperationManagerTest::startOperation(QString opType) {
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, opType));
    m_EventBus->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);

    const mafCore::mafObjectBase *op = NULL;
    QGenericReturnArgument ret_val = mafEventReturnArgument(const mafCore::mafObjectBase*, op);
    m_EventBus->notifyEvent("maf.local.resources.operation.currentRunning", mafEventTypeLocal, NULL, &ret_val);
    return op;
}

const mafExecutionPool *mafOperationManagerTest::retrievePool() {
    const mafExecutionPool *executionPool = NULL;
    QGenericReturnArgument pool_val = mafEventReturnArgument(const mafExecutionPool *, executionPool);
    m_EventBus->notifyEvent("maf.local.resources.operation.executionPool", mafEventTypeLocal, NULL, &pool_val);
    return executionPool;
}


void mafOperationManagerTest::mafOperationManagerAllocationTest() {
    QVERIFY(m_OperationManager != NULL);

    m_ExecutionPool = this->retrievePool();
    QVERIFY(m_ExecutionPool != NULL);

    int poolSize = m_ExecutionPool->size();
    QVERIFY(poolSize == 0);
}

void mafOperationManagerTest::abortExecutionTest() {
    return;

    // Event loop needed for signal notification in threads.
    // ALL THE NOTIFICATIONS HAS TO BE DONE INTO THE EVENT LOOP SCOPE.
    QEventLoop loop;

    const mafCore::mafObjectBase *op = this->startOperation("testEndlessOperation");

    // Start the operation's execution
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    // Print debug message (possible and done immediately because the operation execute in background).
    qDebug() << mafTr("start background execution for ") << op->objectName();

    // Create a timer to abort the endlass loop after a fixed amount of time.
    QTimer t;
    t.setSingleShot(true);

    // Ask for the execution pool
    int poolSize = m_ExecutionPool->size();
    QVERIFY(poolSize == 1);

    // Get the operation's worker.
    mafObjectBase *obj = m_ExecutionPool->at(0);
    mafOperationWorker *worker = qobject_cast<mafResources::mafOperationWorker *>(obj);

    // Connect the timeout to the abort slot to abort the operation's execution.
    connect(&t, SIGNAL(timeout()), worker, SLOT(abortExecution()));
    // The timer also quit the event loop.
    connect(&t, SIGNAL(timeout()), &loop, SLOT(quit()));

    QVERIFY(worker != NULL);

    // Start the timer and the event loop.
    t.start(2000);
    loop.exec();

    // The endless operation has been aborted => removed from the execution pool.
    poolSize = m_ExecutionPool->size();
    QVERIFY(poolSize == 0);
}

bool startOperationOnEventLoop(mafOperationManagerTest *opManagerTest);

void mafOperationManagerTest::undoRedoExecutionTest() {
    // Event loop needed for signal notification in threads.
    // ALL THE NOTIFICATIONS HAS TO BE DONE INTO THE EVENT LOOP SCOPE.
//    QEventLoop loop; <-

//    mafRegisterLocalCallback("maf.local.resources.operation.executed", m_LoopManager, "workDoneCounter()") <-
    m_LoopManager->setCounter(2);

//    QTimer t1, t2;
//    t1.setSingleShot(true);
//    t2.setSingleShot(true);

//    connect(m_LoopManager, SIGNAL(terminateLoop()), &loop, SLOT(quit())); <-
//    connect(&t1, SIGNAL(timeout()), m_LoopManager, SLOT(startOperation()));
//    connect(&t2, SIGNAL(timeout()), m_LoopManager, SLOT(startOperation()));

//    const mafCore::mafObjectBase *op = this->startOperation("testUndoOperation");
    // Start the operation's execution
//    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    // Print debug message (possible and done immediately because the operation execute in background).
//    qDebug() << mafTr("start background execution for ") << op->objectName();

    // Ask for the execution pool
//    int poolSize = m_ExecutionPool->size();
//    QVERIFY(poolSize == 1);

    // Fill the undo stack with other operations...
//    this->startOperation("testUndoOperation");
//    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
//    this->startOperation("testUndoOperation");
//    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
//    this->startOperation("testUndoOperation");
//    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);

//    poolSize = m_ExecutionPool->size();
//    m_LoopManager->setCounter(poolSize);


//    qDebug() << "Starting loop...";
//    t1.start(2000);
//    t2.start(2000);
//    loop.exec();

    // The endless operation has been aborted => removed from the execution pool.
//    poolSize = m_ExecutionPool->size();
//    QVERIFY(poolSize == 0);
    bool res = startOperationOnEventLoop(this);

    QVERIFY(res);

    qDebug() << "****************** Execution ended ******************";
    int undoStackSize;
    QGenericReturnArgument ret_val = mafEventReturnArgument(int, undoStackSize);
    m_EventBus->notifyEvent("maf.local.resources.operation.sizeUndoStack", mafEventTypeLocal, NULL, &ret_val);
    QVERIFY(undoStackSize == 2);
}

bool startOperationOnEventLoop(mafOperationManagerTest *opManagerTest) {
    QEventLoop loop; // Event loop needed for the timer.
    QTimer timerLoop; // Timer that will terminate the event loop.
    timerLoop.setSingleShot(true);
    QObject::connect(&timerLoop, SIGNAL(timeout()), &loop, SLOT(quit()));
    timerLoop.start(4000);

    const mafCore::mafObjectBase *op = opManagerTest->startOperation("testUndoOperation");
    ((QObject *)op)->setObjectName("Operation_1");
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    op = opManagerTest->startOperation("testUndoOperation");
    ((QObject *)op)->setObjectName("Operation_2");
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);

    loop.exec();
    return true;
}


///////////////////////////////
testEventLoopTerminator::testEventLoopTerminator(mafOperationManagerTest *testClass) : QObject(), m_Counter(0), m_TestClass(testClass) {
}

void testEventLoopTerminator::setCounter(int c) {
    m_Counter = c;
}

void testEventLoopTerminator::startOperation() {
    ++m_Counter;
    const mafCore::mafObjectBase *op = m_TestClass->startOperation("testUndoOperation");
    QString n("Operation_");
    n.append(QString::number(m_Counter));
    ((QObject *)op)->setObjectName(n);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    qDebug() << mafTr("start background execution for ") << op->objectName();
}

void testEventLoopTerminator::workDoneCounter() {
    QMutexLocker locker(&mutex);
    --m_Counter;

    qDebug() << "Loop Terminator Counter = " << m_Counter;

    if ( m_Counter == 0 ) {
        qDebug() << "Terminating Loop...";
        emit terminateLoop();
    }
}


MAF_REGISTER_TEST(mafOperationManagerTest);
#include "mafOperationManagerTest.moc"
