/*
 *  mafDataPipeTest.cpp
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
#include <mafDataPipe.h>
#include <mafResourcesRegistration.h>
#include <mafVME.h>

using namespace mafCore;
using namespace mafResources;

//------------------------------------------------------------------------------------------
/**
 Class name: testDataPipeCustom
 This class implements the data pipe to be tested.
 */
class  testDataPipeCustom : public  mafDataPipe {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafDataPipe);

public:
    /// Object constructor.
    testDataPipeCustom(const mafString code_location = "");

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

testDataPipeCustom::testDataPipeCustom(const mafString code_location) : mafDataPipe(code_location), m_PipeLine("") {
}

void testDataPipeCustom::createPipe() {
    m_PipeLine = "Created";
}

void testDataPipeCustom::updatePipe(double t) {
    m_PipeLine = "Updated";
    m_PipeLine.append(mafString::number(t));
    if(m_DecoratorPipe) {
        m_DecoratorPipe->updatePipe(t);
    }
}

//------------------------------------------------------------------------------------------

/**
 Class name: mafDataPipeTest
 This class implements the test suite for mafDataPipe.
 */
class mafDataPipeTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_DataPipe = mafNEW(testDataPipeCustom);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_DataPipe);
    }

    /// mafDataPipe allocation test case.
    void mafDataPipeAllocationTest();
    /// Test the creation and update methods..
    void mafDataPipeCreationAndUpdateTest();

    /// Test the data pipe decoration mechanism.
    void decorateTest();

    void addRemoveInput();

private:
    testDataPipeCustom *m_DataPipe; ///< Test var.
};

void mafDataPipeTest::mafDataPipeAllocationTest() {
    QVERIFY(m_DataPipe != NULL);
}

void mafDataPipeTest::mafDataPipeCreationAndUpdateTest() {
    mafString res("Created");
    m_DataPipe->createPipe();

    QCOMPARE(m_DataPipe->pipeline(), res);

    res = "Updated1";
    m_DataPipe->updatePipe(1);
    QCOMPARE(m_DataPipe->pipeline(), res);
}

void mafDataPipeTest::decorateTest() {
    // Instantiate and create the decorator data pipe.
    testDataPipeCustom *dpDecorator = mafNEW(testDataPipeCustom);
    dpDecorator->createPipe();
    mafString res("Updated1");
    mafDataPipe *dp = m_DataPipe->decorateWithDataPipe(dpDecorator);
    QVERIFY(dp == dpDecorator);

    m_DataPipe->updatePipe(1);
    QCOMPARE(dpDecorator->pipeline(), res);
    mafDEL(dpDecorator);
}

void mafDataPipeTest::addRemoveInput() {
    int num = 0;

    mafDataSet *data1 = mafNEW(mafResources::mafDataSet);
    mafDataSet *data2 = mafNEW(mafResources::mafDataSet);

    mafVME *vme1 = mafNEW(mafResources::mafVME);
    vme1->dataSetCollection()->insertItem(data1);

    mafVME *vme2 = mafNEW(mafResources::mafVME);
    vme2->dataSetCollection()->insertItem(data2);

    m_DataPipe->createPipe();

    // Check if vme1 has been added.
    m_DataPipe->addInput(vme1);
    num = m_DataPipe->inputList()->length();
    QVERIFY(num == 1);

    // Check if vme2 has been added.
    m_DataPipe->addInput(vme2);
    num = m_DataPipe->inputList()->length();
    QVERIFY(num == 2);

    // Check if vme1 has been removed.
    m_DataPipe->removeInput(0);
    num = m_DataPipe->inputList()->length();
    QVERIFY(num == 1);

    // Get the vme2
    mafVME *vme = m_DataPipe->inputList()->at(0);
    QCOMPARE(vme, vme2);

    // Delete VMEs: they will removed from inputList
    mafDEL(data1);
    mafDEL(data2);
    mafDEL(vme1);
    mafDEL(vme2);

    num = m_DataPipe->inputList()->length();
    QVERIFY(num == 0);

}

MAF_REGISTER_TEST(mafDataPipeTest);
#include "mafDataPipeTest.moc"
