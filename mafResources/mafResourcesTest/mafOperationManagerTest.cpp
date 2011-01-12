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
#include <mafOperationManager.h>
#include <mafOperation.h>
#include <mafVME.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

//----------------------------------------------------------------------------------------------------
/**
 Class name: testVMEforOperationManager
 This operation that implements the 'acceptObject' validation function.
 */
class testOperationforOperationManager : public mafResources::mafOperation {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafOperation);

public:
    /// Object constructor.
    testOperationforOperationManager(const mafString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

    /// Return the value of the int parameter
    int paramInt();

    /// Return the value of the double parameter.
    double paramDouble();

public slots:
    /// Set parameters of operation.
    /*virtual*/ void setParameters(mafList<mafVariant> parameters);

private:
    int m_ParamInt; ///< first parameter.
    double m_ParamDouble; ///< second parameter.
};

testOperationforOperationManager::testOperationforOperationManager(const mafString code_location) : mafOperation(code_location), m_ParamInt(0), m_ParamDouble(0.0) {
    m_CanUnDo = false;
}

bool testOperationforOperationManager::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = dynamic_cast<mafVME*>(obj);
    if(vme != NULL) {
        return true;
    }
    return false;
}

void testOperationforOperationManager::setParameters(mafList<mafVariant> parameters) {
    m_ParamInt = parameters.at(0).toInt();
    m_ParamDouble = parameters.at(1).toDouble();
}

int testOperationforOperationManager::paramInt() {
    return m_ParamInt;
}

double testOperationforOperationManager::paramDouble() {
    return m_ParamDouble;
}

//----------------------------------------------------------------------------------------------------
/**
 Class name: testFirstUndoableOperationforOperationManager
 This operation that implements the 'acceptObject' validation function.
 */
class testFirstUndoableOperationforOperationManager : public mafResources::mafOperation {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafOperation);

public:
    /// Object constructor.
    testFirstUndoableOperationforOperationManager(const mafString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

    /// undo operation
    /*virtual*/ void unDo(); 

    /// redo operation
    /*virtual*/ void reDo(); 

};

testFirstUndoableOperationforOperationManager::testFirstUndoableOperationforOperationManager(const mafString code_location) : mafOperation(code_location) {
    m_CanUnDo = true;
}

bool testFirstUndoableOperationforOperationManager::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = dynamic_cast<mafVME*>(obj);
    if(vme != NULL) {
        return true;
    }
    return false;
}

void testFirstUndoableOperationforOperationManager::unDo() {
    mafMsgDebug() << "Undo of testFirstUndoableOperationforOperationManager";
}

void testFirstUndoableOperationforOperationManager::reDo() {
    mafMsgDebug() << "Redo of testFirstUndoableOperationforOperationManager";
}

/**
 Class name: testSecondUndoableOperationforOperationManager
 This operation that implements the 'acceptObject' validation function.
 */
class testSecondUndoableOperationforOperationManager : public mafResources::mafOperation {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafOperation);

public:
    /// Object constructor.
    testSecondUndoableOperationforOperationManager(const mafString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

    /// undo operation
    /*virtual*/ void unDo();

    /// redo operation
    /*virtual*/ void reDo();
};

testSecondUndoableOperationforOperationManager::testSecondUndoableOperationforOperationManager(const mafString code_location) : mafOperation(code_location) {
    m_CanUnDo = true;
}

bool testSecondUndoableOperationforOperationManager::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = dynamic_cast<mafVME*>(obj);
    if(vme != NULL) {
        return true;
    }
    return false;
}

void testSecondUndoableOperationforOperationManager::unDo() {
    mafMsgDebug() << "Undo of testSecondUndoableOperationforOperationManager";
}

void testSecondUndoableOperationforOperationManager::reDo() {
    mafMsgDebug() << "Redo of testSecondUndoableOperationforOperationManager";
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
        m_OperationManager = mafOperationManager::instance();

        mafRegisterObjectAndAcceptBind(testOperationforOperationManager);
        mafRegisterObjectAndAcceptBind(testFirstUndoableOperationforOperationManager);
        mafRegisterObjectAndAcceptBind(testSecondUndoableOperationforOperationManager);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafUnregisterObjectAndAcceptUnbind(testOperationforOperationManager);
        mafUnregisterObjectAndAcceptUnbind(testFirstUndoableOperationforOperationManager);
        mafUnregisterObjectAndAcceptUnbind(testSecondUndoableOperationforOperationManager);

        // Shutdown eventbus singleton and core singletons.
        mafEventBusManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafOperationManager allocation test case.
    void mafOperationManagerAllocationTest();

    /// Test for setParameters.
    void setParametersTest();

    /// Test for startOperation.
    void startOperationTest();

    /// test vme selected
    void vmeSelectedTest();

    /// execute current operation
    void executeOperationTest();

    /// stop current operation
    //void stopOperationTest();

    /// handle the undo stack removing last operation effect
    void undoOperationTest();

    /// reapply operation results
    void redoOperationTest();

    /// undo stack  stress test
    void undoStackStressTest();

private:
    mafEventBusManager *m_EventBus; ///< Reference to the event bus.
    mafOperationManager *m_OperationManager;
};

void mafOperationManagerTest::mafOperationManagerAllocationTest() {
    QVERIFY(m_OperationManager != NULL);
}

void mafOperationManagerTest::setParametersTest() {
    // Create the event's parameters.
    mafEventArgumentsList argList;
    mafString op("testOperationforOperationManager");
    argList.append(mafEventArgument(mafString, op));
    // Notify the event.
    m_EventBus->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);

    mafList<mafVariant> op_params;
    op_params.append(mafVariant(5));
    op_params.append(mafVariant(3.5));

    argList.clear();
    argList.append(mafEventArgument(mafList<mafVariant>, op_params));
    m_EventBus->notifyEvent("maf.local.resources.operation.setParameters", mafEventTypeLocal, &argList);

    const mafCore::mafObjectBase *curr_op = NULL;
    mafGenericReturnArgument ret_val = mafEventReturnArgument(const mafCore::mafObjectBase *, curr_op);
    m_EventBus->notifyEvent("maf.local.resources.operation.currentRunning", mafEventTypeLocal, NULL, &ret_val);
    int vi = ((testOperationforOperationManager *)curr_op)->paramInt();
    double vd = ((testOperationforOperationManager *)curr_op)->paramDouble();

    QVERIFY(vi != 0);
    QVERIFY(vd != 0);

    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
}

void mafOperationManagerTest::startOperationTest() {
    // Create event parameters
    mafEventArgumentsList argList;
    mafString op("mafResources::mafOperation");
    argList.append(mafEventArgument(mafString, op));
    // notify the event.
    m_EventBus->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);

    const mafCore::mafObjectBase *curr_op = NULL;
    mafGenericReturnArgument ret_val = mafEventReturnArgument(const mafCore::mafObjectBase *, curr_op);
    m_EventBus->notifyEvent("maf.local.resources.operation.currentRunning", mafEventTypeLocal, NULL, &ret_val);

    QVERIFY(curr_op != NULL);
}

void mafOperationManagerTest::vmeSelectedTest() {
    mafStringList binding_class_list;

    // Create two test objects.
    mafVME *vme = mafNEW(mafResources::mafVME);

    // Check that the 'obj' showld not be valid and 'vme' yes instead.
    binding_class_list = mafCoreRegistration::acceptObject(vme);
    int num = binding_class_list.count();
    QVERIFY(num != 0);
    QVERIFY(binding_class_list.contains("testOperationforOperationManager"));
    mafDEL(vme);
}

void mafOperationManagerTest::executeOperationTest() {
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);

    const mafCore::mafObjectBase *op = NULL;
    mafGenericReturnArgument ret_val = mafEventReturnArgument(const mafCore::mafObjectBase *, op);
    m_EventBus->notifyEvent("maf.local.resources.operation.currentRunning", mafEventTypeLocal, NULL, &ret_val);

    QVERIFY(op == NULL);
    m_EventBus->notifyEvent("maf.local.resources.operation.clearUndoStack", mafEventTypeLocal);
}

/*void mafOperationManagerTest::stopOperationTest() {
    //test only when develop threaded opertions
    //m_OperationManager->stopOperation();
    //const mafOperation *op = m_OperationManager->currentOperation();

    //QVERIFY(true);
}*/

void mafOperationManagerTest::undoOperationTest() {
    // Start test operation using explicit calls

    mafString vt("testFirstUndoableOperationforOperationManager");
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafString, vt));
    m_EventBus->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);

    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);

    argList.clear();
    // Starts and execute another operation using the 'batch mode' call.
    mafList<mafVariant> params; // no parameters needed.
    params.append("testSecondUndoableOperationforOperationManager");
    mafList<mafVariant> args;
    params.push_back(args);

    argList.push_back(mafEventArgument(mafList<mafVariant>, params));
    //m_OperationManager->executeWithParameters(params);
    m_EventBus->notifyEvent("maf.local.resources.operation.executeWithParameters", mafEventTypeLocal, &argList);

    mafString vt2("testFirstUndoableOperationforOperationManager");
    argList.clear();
    argList.append(mafEventArgument(mafString, vt2));
    m_EventBus->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);

    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);

    m_EventBus->notifyEvent("maf.local.resources.operation.undo", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.undo", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.undo", mafEventTypeLocal);

    m_EventBus->notifyEvent("maf.local.resources.operation.clearUndoStack", mafEventTypeLocal);
}

void mafOperationManagerTest::redoOperationTest() {
    mafString vt("testFirstUndoableOperationforOperationManager");
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafString, vt));
    m_EventBus->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);

    mafString vt2("testFirstUndoableOperationforOperationManager");
    argList.clear();
    argList.append(mafEventArgument(mafString, vt2));
    m_EventBus->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);

    mafString vt3("testFirstUndoableOperationforOperationManager");
    argList.clear();
    argList.append(mafEventArgument(mafString, vt3));
    m_EventBus->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);

    m_EventBus->notifyEvent("maf.local.resources.operation.undo", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.undo", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.undo", mafEventTypeLocal);

    m_EventBus->notifyEvent("maf.local.resources.operation.redo", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.redo", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.redo", mafEventTypeLocal);

    m_EventBus->notifyEvent("maf.local.resources.operation.clearUndoStack", mafEventTypeLocal);
}

void mafOperationManagerTest::undoStackStressTest() {
    mafString vt("testFirstUndoableOperationforOperationManager");
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafString, vt));
    m_EventBus->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);

    int value;
    mafGenericReturnArgument ret_val = mafEventReturnArgument(int, value);
    m_EventBus->notifyEvent("maf.local.resources.operation.sizeUndoStack", mafEventTypeLocal, NULL, &ret_val);
    QVERIFY(value == 1);

    mafString vt2("testSecondUndoableOperationforOperationManager");
    argList.clear();
    argList.append(mafEventArgument(mafString, vt2));
    m_EventBus->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.sizeUndoStack", mafEventTypeLocal, NULL, &ret_val);
    QVERIFY(value == 2);

    mafString vt3("testFirstUndoableOperationforOperationManager");
    argList.clear();
    argList.append(mafEventArgument(mafString, vt3));
    m_EventBus->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.sizeUndoStack", mafEventTypeLocal, NULL, &ret_val);
    QVERIFY(value == 3);


    mafString vt4("testFirstUndoableOperationforOperationManager");
    argList.clear();
    argList.append(mafEventArgument(mafString, vt4));
    m_EventBus->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.sizeUndoStack", mafEventTypeLocal, NULL, &ret_val);
    QVERIFY(value == 4);

    mafString vt5("testSecondUndoableOperationforOperationManager");
    argList.clear();
    argList.append(mafEventArgument(mafString, vt5));
    m_EventBus->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.sizeUndoStack", mafEventTypeLocal, NULL, &ret_val);
    QVERIFY(value == 5);

    mafString vt6("testFirstUndoableOperationforOperationManager");
    argList.clear();
    argList.append(mafEventArgument(mafString, vt6));
    m_EventBus->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.sizeUndoStack", mafEventTypeLocal, NULL, &ret_val);
    QVERIFY(value == 6);

    m_EventBus->notifyEvent("maf.local.resources.operation.undo", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.undo", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.undo", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.undo", mafEventTypeLocal);


    m_EventBus->notifyEvent("maf.local.resources.operation.redo", mafEventTypeLocal);

    mafString vt7("testFirstUndoableOperationforOperationManager");
    argList.clear();
    argList.append(mafEventArgument(mafString, vt7));
    m_EventBus->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);
    m_EventBus->notifyEvent("maf.local.resources.operation.execute", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.sizeUndoStack", mafEventTypeLocal, NULL, &ret_val);
    QVERIFY(value == 4);

    m_EventBus->notifyEvent("maf.local.resources.operation.clearUndoStack", mafEventTypeLocal);
    m_EventBus->notifyEvent("maf.local.resources.operation.sizeUndoStack", mafEventTypeLocal, NULL, &ret_val);
    QVERIFY(value == 0);

}

MAF_REGISTER_TEST(mafOperationManagerTest);
#include "mafOperationManagerTest.moc"
