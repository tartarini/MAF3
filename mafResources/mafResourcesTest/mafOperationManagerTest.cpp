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
#include <mafResourcesRegistration.h>
#include <mafEventBusManager.h>
#include <mafVMEManager.h>
#include <mafOperationManager.h>
#include <mafOperation.h>
#include <mafVME.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

/**
 Class name: testHeavyOperationforOperationManager
 This operation is an example of heavy operation that needs to be executed on a separate thread.
 */
class testHeavyOperationforOperationManager : public mafResources::mafOperation {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafOperation);

public:
    /// Object constructor.
    testHeavyOperationforOperationManager(const QString code_location = "");

public slots:
    /// execution method
    /*virtual*/ void execute();

private:
    int m_val;
};

testHeavyOperationforOperationManager::testHeavyOperationforOperationManager(const QString code_location) : mafOperation(code_location), m_val(0) {
    m_CanUnDo = false;
}

void testHeavyOperationforOperationManager::execute() {
    for ( int i = 0; i < 10000; ++i ) {
        if ( i % 1000 == 0 ) {
            m_val += i;
            qDebug() << m_val;
        }
    }

    emit executionEnded();
}

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
        // Register all the creatable objects for the mafResources module.
        mafResourcesRegistration::registerResourcesObjects();
        m_EventBus = mafEventBusManager::instance();
        m_VMEManager = mafVMEManager::instance();
        m_OperationManager = mafOperationManager::instance();

        m_Vme = mafNEWFromString("mafResources::mafVME");

        mafRegisterObject(testHeavyOperationforOperationManager);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        m_OperationManager->shutdown();

        mafUnregisterObject(testHeavyOperationforOperationManager);

        mafDEL(m_Vme);

        // Shutdown eventbus singleton and core singletons.
        m_VMEManager->shutdown();

        //restore vme manager status
        m_EventBus->notifyEvent("maf.local.resources.hierarchy.create");

        mafEventBusManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafOperationManager allocation test case.
    void mafOperationManagerAllocationTest();

    /// Threaded execution test
    void threadedExecutionTest();

private:
    mafEventBusManager *m_EventBus; ///< Reference to the event bus.
    mafOperationManager *m_OperationManager;
    mafVMEManager *m_VMEManager;
    mafCore::mafObjectBase *m_Vme;
};

void mafOperationManagerTest::mafOperationManagerAllocationTest() {
    QVERIFY(m_OperationManager != NULL);
}

void mafOperationManagerTest::threadedExecutionTest() {
    QString vt("testHeavyOperationforOperationManager");
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, vt));
    m_EventBus->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);

    const mafCore::mafObjectBase *op = NULL;
    QGenericReturnArgument ret_val = mafEventReturnArgument(const mafCore::mafObjectBase*, op);
    m_EventBus->notifyEvent("maf.local.resources.operation.currentRunning", mafEventTypeLocal, NULL, &ret_val);

    QVariant val(true);
    ((mafCore::mafObjectBase *)op)->setProperty("executeOnThread", val);

    QEventLoop loop; // Event loop needed for the threaded execution signal emit.
    QObject::connect(op, SIGNAL(executionEnded()), &loop, SLOT(quit()));

    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    qDebug() << mafTr("start execution in background...");

    loop.exec();
}

MAF_REGISTER_TEST(mafOperationManagerTest);
#include "mafOperationManagerTest.moc"
