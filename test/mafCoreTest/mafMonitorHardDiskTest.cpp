/*
 *  mafMonitorStorageTest.cpp
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 27/01/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafMonitorHardDisk.h>

using namespace mafCore;

//! <title>
//mafMonitorHardDiskTest
//! </title>
//! <description>
//mafMonitorHardDiskTest define a modality of visiting object checking the hash code
//giving in input from the constructor of the visitor
//! </description>

/**
 Class name: mafMonitorHardDiskTest
 This class implements the test suite for mafMonitorHardDisk.
 */
class mafMonitorHardDiskTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
    }

    /// register new object in factory test case.
    void storageTest();

private:
};

void mafMonitorHardDiskTest::storageTest() {
    //! <snippet>
    mafMonitorHardDisk *monitor = new mafMonitorHardDisk();
    //! </snippet>
    QVERIFY(monitor != NULL);
    monitor->update();
    long long totBytes = monitor->property("Total").toLongLong();
    long long freeSpace = monitor->property("Free").toLongLong();
    long long usedSpace = monitor->property("Used").toLongLong();

    mafMessageHandler *mh = mafMessageHandler::instance();
    mh->testSuiteLogMode(true);
    qDebug() << "Total (MB):" << monitor->conversionFromBytes(totBytes, mafMonitorMemory::toMegaBytes);
    qDebug() << "Total (GB):" << monitor->conversionFromBytes(totBytes, mafMonitorMemory::toGigaBytes);
    qDebug() << "Free (MB):" << monitor->conversionFromBytes(freeSpace, mafMonitorMemory::toMegaBytes);
    qDebug() << "Free (GB):" << monitor->conversionFromBytes(freeSpace, mafMonitorMemory::toGigaBytes);
    qDebug() << "Used (MB):" << monitor->conversionFromBytes(usedSpace, mafMonitorMemory::toMegaBytes);
    qDebug() << "Used (GB):" << monitor->conversionFromBytes(usedSpace, mafMonitorMemory::toGigaBytes);
    mh->testSuiteLogMode(false);
    mafDEL(monitor);
}

MAF_REGISTER_TEST(mafMonitorHardDiskTest);
#include "mafMonitorHardDiskTest.moc"



