/*
 *  mafMonitorMemoryTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafMonitorMemory.h>

//forward declaration
class testMonitorMemoryCustom;

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
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// register new object in factory test case.
    void memoryTest();

private:
};
