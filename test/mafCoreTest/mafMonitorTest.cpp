/*
 *  mafMonitorTest.cpp
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 27/01/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

using namespace mafCore;

//-------------------------------------------------------------------------
/**
 Class name: mafVisitorCustom
 Define a custom visitor for testing 'acceptVisitor' method.
 */
class testMonitorImplement : public mafMonitor {
    Q_OBJECT
public:
    testMonitorImplement(const QString code_location = "");

    /// update space variables
    /*virtual*/ void update();

private:
};

testMonitorImplement::testMonitorImplement(const QString code_location) : mafMonitor(code_location) {
}

void testMonitorImplement::update() {
    m_ResultBuffer = "Test";
}

//-------------------------------------------------------------------------


void mafMonitorTest::initTestCase() {
    m_Monitor = mafNEW(testMonitorImplement);
}

void mafMonitorTest::cleanupTestCase() {
    mafDEL(m_Monitor);
}

void mafMonitorTest::monitorTest() {
    QVERIFY(m_Monitor != NULL);

    m_Monitor->update();
    QString res("Test");
    QCOMPARE(*m_Monitor->resultBuffer(), res);
}

#include "mafMonitorTest.moc"


