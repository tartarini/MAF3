/*
 *  mafMonitorMemoryTest.cpp
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

//------------------------------------------------------------------------------------------
/**
 Class name: testMonitorMemoryCustom
 This class implements the visitor to be tested.
 */
class  testMonitorMemoryCustom : public  mafMonitorMemory {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafMonitorMemory);

public:
    /// Object constructor.
    testMonitorMemoryCustom(const QString code_location = "");

    /// implement visit method
    void update();
};

testMonitorMemoryCustom::testMonitorMemoryCustom(const QString code_location) : mafMonitorMemory(code_location) {
}

void testMonitorMemoryCustom::update() {
    m_TotalSpace = 1000;
    m_FreeSpace = 300;
}

void mafMonitorMemoryTest::initTestCase() {
}

void mafMonitorMemoryTest::cleanupTestCase() {
}

void mafMonitorMemoryTest::memoryTest() {
    //! <snippet>
    mafMonitorMemory *monitor = new testMonitorMemoryCustom();
    //! </snippet>
    QVERIFY(monitor != NULL);
    monitor->update();
    qDebug() << "Total:" << monitor->property("Total").toLongLong();
    qDebug() << "Free:" <<monitor->property("Free").toLongLong();
    qDebug() << "Used:" <<monitor->property("Used").toLongLong();

    QVERIFY(monitor->totalSpace() == 1000);
    QVERIFY(monitor->freeSpace() == 300);

    mafDEL(monitor);
}

#include "mafMonitorMemoryTest.moc"


