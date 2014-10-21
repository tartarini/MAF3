/*
 *  mafProfilerTest.h
 *  mafQATest
 *
 *  Created by Daniele Giunchi on 10/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include "mafProfiler.h"


/**
 Class name: mafProfilerTest
 This class implements the test suite for mafProfiler.
 */

//! <title>
//mafProfiler
//! </title>
//! <description>
//mafProfiler allows calculating timing performances of blocks or code between two "Start" and "Stop" consecutive calls.
//! </description>
class mafProfilerTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// mafProfiler allocation test case.
    void mafProfilerAllocationTest();
    /// Test profiler
    void profilerTest();

private:
    mafProfiler *m_Profiler; ///< Test var.
};
