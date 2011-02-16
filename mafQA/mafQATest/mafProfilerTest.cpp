/*
 *  mafProfilerTest.cpp
 *  mafQATest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include "mafProfiler.h"

using namespace mafCore;
using namespace mafQA;

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

private slots:
    /// Initialize test variables
    void initTestCase() {
        //! <snippet>
        m_Profiler = mafProfiler::instance();
        //! </snippet>
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        m_Profiler->shutdown();
    }

    /// mafProfiler allocation test case.
    void mafProfilerAllocationTest();
    /// Test profiler
    void profilerTest();

private:
    mafProfiler *m_Profiler; ///< Test var.
};

void mafProfilerTest::mafProfilerAllocationTest() {
    QVERIFY(m_Profiler != NULL);
}

long int calculate(long int v) {
    return (v + 5) * 32 - 15;
}


void mafProfilerTest::profilerTest() {
    //! <snippet>
    m_Profiler->profilerStart("profilerTest", "mafProfilerTest.cpp", "Test note");
    long int m = 0;
    for(int i = 0; i < 100000; ++i) {
        m = calculate(m);
    }
    m_Profiler->profilerStop("profilerTest", "mafProfilerTest.cpp");
    //! </snippet>

    m_Profiler->profilerStart("profilerTest2", "mafProfilerTest.cpp", "Test note");
    m = 0;
    for(int i = 0; i < 1000000; ++i) {
        m = calculate(m);
    }
    m_Profiler->profilerStop("profilerTest2", "mafProfilerTest.cpp");

    mafString *buff = m_Profiler->bufferString();

    mafMsgDebug() << *buff;
    QVERIFY(buff->length() > 0);
}


MAF_REGISTER_TEST(mafProfilerTest);
#include "mafProfilerTest.moc"
