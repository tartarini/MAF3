/*
 *  mafQAProfilerTest.cpp
 *  mafQATest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 22/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafQATestList.h"

using namespace mafCore;
using namespace mafQA;

void mafProfilerTest::initTestCase() {
    //! <snippet>
    m_Profiler = mafProfiler::instance();
    //! </snippet>
}

void mafProfilerTest::cleanupTestCase() {
    m_Profiler->shutdown();
}

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

    QString *buff = m_Profiler->bufferString();

    qDebug() << *buff;
    QVERIFY(buff->length() > 0);
}

#include "mafProfilerTest.moc"
