/*
 *  mafOperationTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafOperation.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;


/**
 Class name: testOperation
 This is an example of operation.
 */
class testOperation : public mafResources::mafOperation {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafOperation);
    
public:
    /// Object constructor.
    testOperation(const QString code_location = "");
    
protected:
    /// Terminate the execution.
    /*virtual*/ void terminated();
    
public Q_SLOTS:
    /// execution method
    /*virtual*/ void execute();
    
};

testOperation::testOperation(const QString code_location) : mafOperation(code_location) {
    setObjectName("testOperation");
}

void testOperation::execute() {
    Q_EMIT executionEnded();
}

void testOperation::terminated() {
    
}

/**
 Class name: mafOperationTest
 This class implements the test suite for mafOperation.
 */

//! <title>
//mafOperation
//! </title>
//! <description>
//mafOperation  provides basic API for building elaboration algorithms for mafResources.
//An operation takes as input one or more mafVMEs and generate as output a mafVME.
//The algorithm executed on the input data is provided ad mafPipeData. The mafOperation
//that manage the possibility to have the undo mechanism for the executed algorithm.
//! </description>
class mafOperationTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafRegisterObject(testOperation);
        m_Operation = mafNEW(testOperation);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Operation);
        mafEventBus::mafEventBusManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafOperation allocation test case.
    void mafOperationAllocationTest();

private:
    mafOperation *m_Operation; ///< Test var.
};

void mafOperationTest::mafOperationAllocationTest() {
    QVERIFY(m_Operation != NULL);
    QVERIFY(m_Operation->canUnDo());

    QVERIFY ( m_Operation->initialize() );
    m_Operation->unDo();
    m_Operation->reDo();
    m_Operation->terminate();
    QVERIFY ( !m_Operation->isRunning() );
}

MAF_REGISTER_TEST(mafOperationTest);
#include "mafOperationTest.moc"
