/*
 *  mafMonitorTest.cpp
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 27/01/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafMonitor.h>

using namespace mafCore;

//-------------------------------------------------------------------------
/**
 Class name: mafVisitorCustom
 Define a custom visitor for testing 'acceptVisitor' method.
 */
class testMonitorImplement : public mafMonitor {
public:
    testMonitorImplement(const mafString code_location = "");

    /// update space variables
    /*virtual*/ void update();

private:
};

testMonitorImplement::testMonitorImplement(const mafString code_location) : mafMonitor(code_location) {
}

void testMonitorImplement::update() {
    m_ResultBuffer = "Test";
}

//-------------------------------------------------------------------------


/**
 Class name: mafMonitorTest
 This class implements the test suite for mafMonitor.
 */
class mafMonitorTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_Monitor = mafNEW(testMonitorImplement);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Monitor);
    }

    /// register new object in factory test case.
    void monitorTest();

private:
    testMonitorImplement *m_Monitor; ///< Test Var.
};

void mafMonitorTest::monitorTest() {
    QVERIFY(m_Monitor != NULL);

    m_Monitor->update();
    mafString res("Test");
    QCOMPARE(*m_Monitor->resultBuffer(), res);
}

MAF_REGISTER_TEST(mafMonitorTest);
#include "mafMonitorTest.moc"


