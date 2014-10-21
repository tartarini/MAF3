/*
 *  mafMonitorRAMTest.cpp
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

void mafMonitorRAMTest::initTestCase() {
    mafCore::mafMessageHandler::instance()->installMessageHandler();
}

void mafMonitorRAMTest::cleanupTestCase() {
    mafCore::mafMessageHandler::instance()->shutdown();
}

void mafMonitorRAMTest::memoryTest() {
    //! <snippet>
    mafMonitorRAM *monitor = new mafMonitorRAM();
    //! </snippet>
    QVERIFY(monitor != NULL);
    monitor->update();
    qDebug() << "Total:" << monitor->property("Total").toLongLong();
    qDebug() << "Free:" <<monitor->property("Free").toLongLong();
    qDebug() << "Used:" <<monitor->property("Used").toLongLong();

    mafDEL(monitor);
}

#include "mafMonitorRAMTest.moc"


