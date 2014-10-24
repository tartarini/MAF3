/*
 *  mafLoggerConsoleTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 27/03/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

using namespace mafCore;

void mafLoggerConsoleTest::initTestCase() {
    //! <snippet>
    m_Logger = mafNEW(mafCore::mafLoggerConsole);
    //! </snippet>
}

void mafLoggerConsoleTest::cleanupTestCase() {
    mafDEL(m_Logger);
}

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

#include "mafLoggerConsoleTest.moc"
