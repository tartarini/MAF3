/*
 *  mafLoggerConsoleTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafLoggerConsole.h>

using namespace mafCore;

/**
 Class name: mafLoggerConsoleTest
 This class implements the test suite for mafLoggerConsole.
 */

//! <title>
//mafLoggerConsole
//! </title>
//! <description>
//mafLoggerConsole defines the MAF3 default logging class that will shows messages to console output.
//It also redifine "virtual void loggedMessage(QtMsgType type, QString &msg)" used to log the given message to the console.
//! </description>

class mafLoggerConsoleTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        //! <snippet>
        m_Logger = mafNEW(mafCore::mafLoggerConsole);
        //! </snippet>
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Logger);
    }

    /// create new object and check that is not NULL test case.
    void mafLoggerConsoleConstructorTest();

    /// log a string into the standard output (console)
    void logMesageTest();

private:
    mafLoggerConsole *m_Logger; ///< Testing variable.
};

void mafLoggerConsoleTest::mafLoggerConsoleConstructorTest() {
    QVERIFY(m_Logger != NULL);
}

void mafLoggerConsoleTest::logMesageTest() {
    //! <snippet>
    QString msg("Test for mafLoggerConsole");
    m_Logger->logMessage(QtDebugMsg, msg);
    //! </snippet>

    // Test QtWarningMsg
    m_Logger->logMessage(QtWarningMsg, msg);

    // ... Critical
    m_Logger->logMessage(QtCriticalMsg, msg);

    // ... and Fatal.
    m_Logger->logMessage(QtFatalMsg, msg);

    // Test suite debug and warning messages...
    m_Logger->setLogMode(mafLogModeTestSuite);
    m_Logger->logMessage(QtDebugMsg, msg);
    m_Logger->logMessage(QtWarningMsg, msg);
}

MAF_REGISTER_TEST(mafLoggerConsoleTest);
#include "mafLoggerConsoleTest.moc"
