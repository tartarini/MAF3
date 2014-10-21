/*
 *  mafMonitorTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafMonitor.h>

using namespace mafCore;

class testMonitorImplement;

/**
 Class name: mafMonitorTest
 This class implements the test suite for mafMonitor.
 */
class mafMonitorTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// register new object in factory test case.
    void monitorTest();

private:
    testMonitorImplement *m_Monitor; ///< Test Var.
};
