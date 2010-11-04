/*
 *  mafPipeTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafPipe.h>
#include <mafVME.h>

using namespace mafCore;
using namespace mafResources;

//------------------------------------------------------------------------------------------
/**
 Class name: testPipeCustom
 This class implements the data pipe to be tested.
 */
class  testPipeCustom : public  mafPipe {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipe);

public:
    /// Object constructor.
    testPipeCustom(const mafString code_location = "");

    /// Initialize and create the pipeline
    /*virtual*/ void createPipe();

    /// Return the string variable initializated and updated from the data pipe.
    mafString pipeline() {return m_PipeLine;}

public slots:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);

private:
    mafString m_PipeLine; ///< Test Var.
};

testPipeCustom::testPipeCustom(const mafString code_location) : mafPipe(code_location), m_PipeLine("") {
}

void testPipeCustom::createPipe() {
    m_PipeLine = "Created";
}

void testPipeCustom::updatePipe(double t) {
    m_PipeLine = "Updated";
    m_PipeLine.append(mafString::number(t));
}
//------------------------------------------------------------------------------------------

/**
 Class name: mafPipeTest
 This class implements the test suite for mafPipe.
 */
class mafPipeTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        m_Pipe = mafNEW(testPipeCustom);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Pipe);
    }

    /// mafPipe allocation test case.
    void mafPipeAllocationTest();
    /// Test the creation and update methods.
    void mafPipeCreationAndUpdateTest();
    /// Test the input management of the mafPipe
    void inputManagementTest();

private:
    testPipeCustom *m_Pipe; ///< Test var.
};

void mafPipeTest::mafPipeAllocationTest() {
    QVERIFY(m_Pipe != NULL);
}

void mafPipeTest::mafPipeCreationAndUpdateTest() {
    mafString res("Created");
    m_Pipe->createPipe();

    QCOMPARE(m_Pipe->pipeline(), res);

    res = "Updated1";
    m_Pipe->updatePipe(1);
    QCOMPARE(m_Pipe->pipeline(), res);
}

void mafPipeTest::inputManagementTest() {
    mafDataSet *data1 = mafNEW(mafResources::mafDataSet);
    mafDataSet *data2 = mafNEW(mafResources::mafDataSet);

    mafVME *vme1 = mafNEW(mafResources::mafVME);
    vme1->dataSetCollection()->insertItem(data1);

    mafVME *vme2 = mafNEW(mafResources::mafVME);
    vme2->dataSetCollection()->insertItem(data2);

    m_Pipe->setInput(vme1);

    int num = m_Pipe->inputList()->length();
    QVERIFY(num == 1);

    // Get the vme1
    // Check if vme has been added
    mafVME *vme = m_Pipe->inputList()->at(0);
    QCOMPARE(vme, vme1);

    // remove the item at index 0
    m_Pipe->removeInput(vme1);
    num = m_Pipe->inputList()->length();
    QVERIFY(num == 0);

    m_Pipe->setInput(vme1);
    // substitute the vme1 with the vme2
    m_Pipe->setInput(vme2);

    // Get the vme2
    // Check if vme has been added.
    vme = m_Pipe->inputList()->at(0);
    QCOMPARE(vme, vme2);

    // try to remove a vme not present in the list
    mafMessageHandler::instance()->testSuiteLogMode(true);
    m_Pipe->removeInput(vme1);
    mafMessageHandler::instance()->testSuiteLogMode(false);
    num = m_Pipe->inputList()->length();
    QVERIFY(num == 1);

    // destroy the VME => will be removed also from the input list of the pipe.
    mafDEL(data1);
    mafDEL(data2);
    mafDEL(vme1);
    mafDEL(vme2);

    // now the input list should be empty.
    num = m_Pipe->inputList()->length();
    QVERIFY(num == 0);
}

MAF_REGISTER_TEST(mafPipeTest);
#include "mafPipeTest.moc"
