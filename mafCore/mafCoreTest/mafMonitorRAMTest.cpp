/*
 *  mafMonitorRAMTest.cpp
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 27/01/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafMonitorRAM.h>

using namespace mafCore;

//! <title>
//mafMonitorRAMTest
//! </title>
//! <description>
//mafMonitorRAMTest define a modality of visiting object checking the hash code
//giving in input from the constructor of the visitor
//! </description>

/**
 Class name: mafMonitorRAMTest
 This class implements the test suite for mafMonitorRAM.
 */
class mafMonitorRAMTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
    }

    /// register new object in factory test case.
    void memoryTest();

private:
};

void mafMonitorRAMTest::memoryTest() {
    //! <snippet>
    mafMonitorRAM *monitor = new mafMonitorRAM();
    //! </snippet>
    QVERIFY(monitor != NULL);
    monitor->update();
    mafMsgDebug() << "Total:" << monitor->property("Total").toLongLong();
    mafMsgDebug() << "Free:" <<monitor->property("Free").toLongLong();
    mafMsgDebug() << "Used:" <<monitor->property("Used").toLongLong();

    mafDEL(monitor);
}

MAF_REGISTER_TEST(mafMonitorRAMTest);
#include "mafMonitorRAMTest.moc"


