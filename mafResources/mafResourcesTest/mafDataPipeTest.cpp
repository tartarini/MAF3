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

MAF_REGISTER_TEST(mafDataPipeTest);
#include "mafDataPipeTest.moc"
