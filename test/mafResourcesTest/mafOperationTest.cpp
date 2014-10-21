/*
 *  mafOperationTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 14/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

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

void mafOperationTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    mafRegisterObject(testOperation);
    m_Operation = mafNEW(testOperation);
}

void mafOperationTest::cleanupTestCase() {
    mafDEL(m_Operation);
    mafEventBus::mafEventBusManager::instance()->shutdown();
    mafMessageHandler::instance()->shutdown();
}

void mafOperationTest::mafOperationAllocationTest() {
    QVERIFY(m_Operation != NULL);
    QVERIFY(m_Operation->canUnDo());

    QVERIFY ( m_Operation->initialize() );
    m_Operation->unDo();
    m_Operation->reDo();
    m_Operation->terminate();
    QVERIFY ( !m_Operation->isRunning() );
}

#include "mafOperationTest.moc"
