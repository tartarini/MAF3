/*
 *  mafMonitorStorageTest.cpp
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

void mafMonitorHardDiskTest::initTestCase() {
}

void mafMonitorHardDiskTest::cleanupTestCase() {
}

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

#include "mafMonitorHardDiskTest.moc"



