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

#include <mafTestSuite.h>
#include <mafMonitorMemory.h>

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

//------------------------------------------------------------------------------------------


//! <title>
//mafMonitorMemoryTest
//! </title>
//! <description>
//mafMonitorMemoryTest define a modality of visiting object checking the hash code
//giving in input from the constructor of the visitor
//! </description>

/**
 Class name: mafMonitorMemoryTest
 This class implements the test suite for mafMonitorMemory.
 */
class mafMonitorMemoryTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
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

MAF_REGISTER_TEST(mafMonitorMemoryTest);
#include "mafMonitorMemoryTest.moc"


