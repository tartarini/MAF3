/*
 *  mafQAManagerTest.cpp
 *  mafQATest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafQAManager.h>

#ifdef WIN32
    #define TEST_LIBRARY_NAME "mafPluginTest_d.dll"
    #define TEST_SCRIPT_NAME "../../../MAF/mafQA/mafQATest/AppExample.bat"
#else
    #ifdef __APPLE__
        #define TEST_LIBRARY_NAME "libmafPluginTest_debug.dylib"
        #define TEST_SCRIPT_NAME "../../../MAF/mafQA/mafQATest/AppExample"
    #else
        #define TEST_LIBRARY_NAME "libmafPluginTest_debug.so"
        #define TEST_SCRIPT_NAME "../../../MAF/mafQA/mafQATest/AppExample.sh"
    #endif
#endif


using namespace mafCore;
using namespace mafQA;

/**
 Class name: mafQAManagerTest
 This class implements the test suite for mafQAManager.
 */
class mafQAManagerTest: public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_QAManager = mafQAManager::instance();
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        m_QAManager->shutdown();
    }

    /// mafQAManager allocation test case.
    void mafQAManagerAllocationTest();
    /// Profile test case
    void profilerTest();
    /// Poll URL test
    void pollUrlTest();
    /// RunPythonScript test case
    void runPythonScriptTest();
    /// RunScript test case
    void runScriptTest();
    /// Memory monitor test case
    void memoryMonitorTest();
    /// Validate Plugin test case
    //void pluginValidateTest();

private:
    mafQAManager *m_QAManager; ///< Test var.
};

void mafQAManagerTest::mafQAManagerAllocationTest() {
    QVERIFY(m_QAManager != NULL);
}

long int calcToProfile(long int v) {
    return (v + 5) * 32 - 15;
}

void mafQAManagerTest::profilerTest() {
    // Initialize the profiler
    m_QAManager->profilerInit();
    // Start the profiler with a string comment.
    m_QAManager->profilerStart("Profile First Run (longer)");

    // snippet of code to be monitored.
    long int m = 0;
    int i = 0;
    for(; i < 500000000; i++) {
        m = calcToProfile(m);
    }

    // Stop the profiler to get the timing information
    m_QAManager->profilerStop();

    m_QAManager->profilerStart("Profile Second Run (shorter)");

    // snippet of code to be monitored.
    m = 0;
    i = 0;
    for(; i < 100000000; i++) {
        m = calcToProfile(m);
    }

    // Stop the profiler to get the timing information
    m_QAManager->profilerStop();

    // Show the results on console.
    m_QAManager->profilerViewResultsOnConsole();
    // Save the results on file with the possibility to open it using default viewer.
    mafString tmp = QDir::tempPath();
    tmp.append("/maf3Logs");
    QDir log_dir(tmp);
    if(!log_dir.exists()) {
        log_dir.mkpath(tmp);
    }
    tmp.append("/profilerLog.txt");

    // Save the result to the file and if uncommented the boolean value
    // open the file with the default viewer.
    m_QAManager->profilerViewResultsOnFile(tmp/*, true*/);

    // Shutdown the profiler.
    m_QAManager->profilerShutdown();
}

void mafQAManagerTest::pollUrlTest() {
    //m_QAManager->openPollUrl("http://www.biomedtown.org/biomed_town/MAF/MAF3%20Floor/Reception/");
}

void mafQAManagerTest::runPythonScriptTest() {
    mafMsgDebug() << "Asynchronous:";
    mafStringList argList;
    int res = m_QAManager->runPythonScript("../../../MAF/mafQA/scriptsQA/GeneratorQA.py",argList);
    QVERIFY(res == 0);

    //launch sync
    mafMsgDebug() << "Synchronous:";
    res = m_QAManager->runPythonScript("../../../MAF/mafQA/scriptsQA/GeneratorQA.py",argList, true);
    QVERIFY(res == 0);
}

void mafQAManagerTest::runScriptTest() {
    /*mafMsgDebug() << "Asynchronous:";
    mafStringList argList;
    int res = m_QAManager->runScript(TEST_SCRIPT_NAME,argList, false);
    QCOMPARE(res, 0);*/

    //launch sync
    mafMsgDebug() << "Synchronous:";
    mafStringList argList;
    int res;
    res = m_QAManager->runScript(TEST_SCRIPT_NAME,argList, true);
    QCOMPARE(res, 0);
}

void mafQAManagerTest::memoryMonitorTest() {
    // Print all monitor result
    m_QAManager->enableHardDiskMonitor(true);
    m_QAManager->enableRAMMonitor(true);
    m_QAManager->printMemoryMonitorResultOnConsole();

    // Print only RAM monitor result
    m_QAManager->enableHardDiskMonitor(false);
    m_QAManager->printMemoryMonitorResultOnConsole();

    // Don't print anything
    m_QAManager->enableHardDiskMonitor(false);
    m_QAManager->enableRAMMonitor(false);
    m_QAManager->printMemoryMonitorResultOnConsole();
}

/*void mafQAManagerTest::pluginValidateTest() {
    bool res = m_QAManager->pluginValidate(TEST_LIBRARY_NAME);
    QVERIFY(res);

    res = m_QAManager->pluginValidate("FakePlugin");
    QVERIFY(!res);
  }*/


MAF_REGISTER_TEST(mafQAManagerTest);
#include "mafQAManagerTest.moc"
