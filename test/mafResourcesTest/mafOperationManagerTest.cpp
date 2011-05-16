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
#include <mafOperationWorker.h>

#define kMAX_COUNT 50000

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

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

protected:
    /// Terminate the execution.
    /*virtual*/ void terminated();
    
public slots:
    /// execution method
    /*virtual*/ void execute();
};

testEndlessOperation::testEndlessOperation(const QString code_location) : mafOperation(code_location) {
    m_CanUnDo = false;
    setObjectName("testEndlessOperation");
}

void testEndlessOperation::execute() {
    while ( m_Status != mafOperationStatusAborted ) {
        ;
    }

    if ( m_Status == mafOperationStatusAborted ) {
        return;
    }

    emit executionEnded();
}

void testEndlessOperation::terminated() {
    
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

protected:
    /// Terminate the execution.
    /*virtual*/ void terminated();
    
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

void testNotUndoOperation::terminated() {
    
}

//------------------------------------------------------------------------------------------------------------------------------------------------

/**
 Class name: testUndoOperation
 This operation is an example of undoable operation.
 */
class testUndoOperation : public mafResources::mafOperation {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafOperation);

public slots:
    /// execution method
    /*virtual*/ void execute();

    /// Allows to call the piece of algorithm that is needed to restore the previous state of the operation's execution.
    /*virtual*/ void unDo();

    /// Allows to call the piece of algorithm that is needed to apply the operation again.
    /*virtual*/ void reDo();

public:
    /// Object constructor.
    testUndoOperation(const QString code_location = "");
    
    /// Return the internal variable value.
    int val();
    
protected:
    /// Terminate the execution.
    /*virtual*/ void terminated();
    
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

    //Notify vme add
    mafVME *vme = mafNEW(mafResources::mafVME);
    vme->setObjectName(mafTr("Test Vme"));
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, vme));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add.test", mafEventTypeLocal, &argList);

    // set the final result value.
    m_Val = kMAX_COUNT;

    qDebug() << this->objectName() << " emitting executionEnded...";
    mafDEL(vme);
    emit executionEnded();
}

void testUndoOperation::unDo() {
    qDebug() << this->objectName() << " performs unDo...";
    m_Val = 0;
}

void testUndoOperation::reDo() {
    qDebug() << this->objectName() << " performs reDo...";
    m_Val = kMAX_COUNT;
}

void testUndoOperation::terminated() {
    
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
        mafRegisterLocalSignal("maf.local.resources.vme.add.test", this, "vmeAddSignalTest(mafCore::mafObjectBase *)")
        mafRegisterLocalCallback("maf.local.resources.vme.add.test", this, "vmeAddTest(mafCore::mafObjectBase *)")
        mafMessageHandler::instance()->installMessageHandler();
        m_EventBus = mafEventBusManager::instance();

        m_VMEManager = mafVMEManager::instance();
        //Select root
        mafObject *root;
        QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObject *, root);
        mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.root", mafEventTypeLocal, NULL, &ret_val);

        m_OperationManager = mafOperationManager::instance();

        // Register all the creatable objects for the mafResources module.
        mafResourcesRegistration::registerResourcesObjects();

        // Register custom operations.
        mafRegisterObject(testEndlessOperation);
        mafRegisterObject(testNotUndoOperation);
        mafRegisterObject(testUndoOperation);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        qDebug() << "cleanup test suite...";
        m_OperationManager->shutdown();
        m_VMEManager->shutdown();

        // Unregister custom operations.
        mafUnregisterObject(testEndlessOperation);
        mafUnregisterObject(testNotUndoOperation);
        mafUnregisterObject(testUndoOperation);

        //restore vme manager status
        m_EventBus->notifyEvent("maf.local.resources.hierarchy.request");

        m_EventBus->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafOperationManager allocation test case.
    void mafOperationManagerAllocationTest();

    /// Start and cancel test case.
    void cancelExecutionTest();

    /// Abort execution test
    void abortExecutionTest();

    /// Undo-Redo functionality test
    void undoRedoExecutionTest();

public:
    /// Utility method to start operations. Return the operation started.
    const mafCore::mafObjectBase *startOperation(QString opType);

    /// Utility method to retrieve the execution pool.
    const mafExecutionPool *retrievePool();

public slots:
    void vmeAddTest(mafCore::mafObjectBase *vme);

signals:
    void vmeAddSignalTest(mafCore::mafObjectBase *vme);

private:
    mafEventBusManager *m_EventBus; ///< Reference to the event bus.
    mafOperationManager *m_OperationManager; ///< Reference to the OperationManager.
    const mafExecutionPool *m_ExecutionPool; ///< Pool of running thread.
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

 void mafOperationManagerTest::vmeAddTest(mafCore::mafObjectBase *vme) {
     QVERIFY(vme);
 }

void mafOperationManagerTest::mafOperationManagerAllocationTest() {
    QVERIFY(m_OperationManager != NULL);

    m_ExecutionPool = this->retrievePool();
    QVERIFY(m_ExecutionPool != NULL);

    int poolSize = m_ExecutionPool->size();
    QVERIFY(poolSize == 0);
}


void mafOperationManagerTest::cancelExecutionTest() {
    const mafCore::mafObjectBase *op = this->startOperation("testEndlessOperation");

    // Cancel the operation's execution
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.stop", mafEventTypeLocal);

    int poolSize = m_ExecutionPool->size();
    QVERIFY(poolSize == 0);
}

void mafOperationManagerTest::abortExecutionTest() {
    const mafCore::mafObjectBase *op = this->startOperation("testEndlessOperation");

    // Start the operation's execution
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    // Print debug message (possible and done immediately because the operation execute in background).
    qDebug() << mafTr("start background execution for ") << op->objectName();

    // Create a timer to abort the endlass loop after a fixed amount of time.
    
    // Get the operation's worker.
    QThread *obj = m_ExecutionPool->at(0);
    mafOperationWorker *worker = qobject_cast<mafResources::mafOperationWorker *>(obj);
    
    QTime dieTime = QTime::currentTime().addSecs(1);
    while(QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }

    worker->abortExecution();

    //loop.exec();
    QTime dieTimeAFter = QTime::currentTime().addSecs(3);
    while(QTime::currentTime() < dieTimeAFter) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 3);
    }


    qDebug() << "****************** Execution ABORTED ******************";

    // The endless operation has been aborted => removed from the execution pool.
    int poolSize = m_ExecutionPool->size();
    QVERIFY(poolSize == 0);
}


void mafOperationManagerTest::undoRedoExecutionTest() {

    QTime dieTime = QTime::currentTime().addSecs(1);
    while(QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }

    const mafCore::mafObjectBase *op = this->startOperation("testUndoOperation");
    ((QObject *)op)->setObjectName("Operation_1");
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    op = this->startOperation("testUndoOperation");
    ((QObject *)op)->setObjectName("Operation_2");
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    op = this->startOperation("testUndoOperation");
    ((QObject *)op)->setObjectName("Operation_3");
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    op = this->startOperation("testUndoOperation");
    ((QObject *)op)->setObjectName("Operation_4");
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    
    
    QTime dieTimeAFter = QTime::currentTime().addSecs(3);
    while(QTime::currentTime() < dieTimeAFter) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 3);
    }

    qDebug() << "****************** Execution ended ******************";
    int undoStackSize;
    QGenericReturnArgument ret_val = mafEventReturnArgument(int, undoStackSize);
    m_EventBus->notifyEvent("maf.local.resources.operation.sizeUndoStack", mafEventTypeLocal, NULL, &ret_val);
    
    QVERIFY(undoStackSize == 4);
    
    m_EventBus->notifyEvent("maf.local.resources.operation.undo", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.undo", mafEventTypeLocal);
    
    m_EventBus->notifyEvent("maf.local.resources.operation.redo", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.undo", mafEventTypeLocal);
    
    op = this->startOperation("testUndoOperation");
    ((QObject *)op)->setObjectName("Operation_5");
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    
    dieTimeAFter = QTime::currentTime().addSecs(3);
    while(QTime::currentTime() < dieTimeAFter) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 3);
    }
    
    ret_val = mafEventReturnArgument(int, undoStackSize);
    m_EventBus->notifyEvent("maf.local.resources.operation.sizeUndoStack", mafEventTypeLocal, NULL, &ret_val);
    
    QVERIFY(undoStackSize == 3);
    
    op = this->startOperation("testNotUndoOperation");
    ((QObject *)op)->setObjectName("Operation_6");
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    
    
    dieTimeAFter = QTime::currentTime().addSecs(3);
    while(QTime::currentTime() < dieTimeAFter) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 3);
    }
    
    qDebug() << "****************** Execution ended ******************";
    ret_val = mafEventReturnArgument(int, undoStackSize);
    m_EventBus->notifyEvent("maf.local.resources.operation.sizeUndoStack", mafEventTypeLocal, NULL, &ret_val);
    
    QVERIFY(undoStackSize == 0);
}


MAF_REGISTER_TEST(mafOperationManagerTest);
#include "mafOperationManagerTest.moc"
