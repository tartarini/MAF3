/*
 *  mafOperationTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
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
 Class name: mafOperationTest
 This class implements the test suite for mafOperation.
 */

//! <title>
//mafOperation
//! </title>
//! <description>
//mafOperation  provides basic API for building elaboration algorithms for mafResources.
//An operation takes as input one or more mafVMEs and generate as output a mafVME.
//The algorithm executed on the input data is provided ad mafDataPipe. The mafOperation
//that manage the possibility to have the undo mechanism for the executed algorithm.
//! </description>
class mafOperationTest: public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        m_Operation = mafNEW(mafResources::mafOperation);
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
    mafOperationType op_type = m_Operation->operationType();
    QVERIFY(op_type == mafOperationTypeOperation);
    QVERIFY(m_Operation->canUnDo());

    m_Operation->initialize();
    QVERIFY ( m_Operation->isRunning() );
    m_Operation->unDo();
    m_Operation->reDo();
    m_Operation->terminate();
    QVERIFY ( !m_Operation->isRunning() );
}

MAF_REGISTER_TEST(mafOperationTest);
#include "mafOperationTest.moc"
