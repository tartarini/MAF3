/*
 *  mafQAManagerTest.h
 *  mafQATest
 *
 *  Created by Daniele Giunchi on 10/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafQAManager.h>

/**
 Class name: mafQAManagerTest
 This class implements the test suite for mafQAManager.
 */
class mafQAManagerTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// mafQAManager allocation test case.
    void mafQAManagerAllocationTest();
    /// Profile test case
    void profilerTest();
    /// Poll URL test
    void pollUrlTest();
#ifndef valgrind_ENABLE
    /// RunPythonScript test case
    void runPythonScriptTest();
    /// RunScript test case
    void runScriptTest();
#endif //valgrind_ENABLE
    /// Memory monitor test case
    void memoryMonitorTest();
    /// Validate Plugin test case
    //void pluginValidateTest();

private:
    mafQAManager *m_QAManager; ///< Test var.
};
