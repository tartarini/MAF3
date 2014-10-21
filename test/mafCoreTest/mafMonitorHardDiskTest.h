/*
 *  mafMonitorStorageTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafMonitorHardDisk.h>

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
    void initTestCase();
    
    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// register new object in factory test case.
    void storageTest();

private:
};
