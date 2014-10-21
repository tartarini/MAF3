/*
 *  mafLoggerBufferTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

using namespace mafCore;

void mafLoggerBufferTest::initTestCase() {
    //! <snippet>
    m_Logger = mafNEW(mafCore::mafLoggerBuffer);
    //! </snippet>
}

void mafLoggerBufferTest::cleanupTestCase() {
    //! <snippet>
    mafDEL(m_Logger);
    //! </snippet>
}

void mafLoggerBufferTest::mafLoggerBufferConstructorTest() {
    QVERIFY(m_Logger != NULL);
}

void mafLoggerBufferTest::logMessageTest() {
    //! <snippet>
    QString msg("Test for mafLoggerBuffer");
    m_Logger->logMessage(QtDebugMsg, msg);
    //! </snippet>
    QString res = "Debug: Test for mafLoggerBuffer\n";
    QString buf = m_Logger->bufferLog();

    QVERIFY(buf.contains(res));

    res = "Warning: Test for mafLoggerBuffer\n";
    m_Logger->logMessage(QtWarningMsg, msg);
    buf = m_Logger->bufferLog();
    QVERIFY(buf.contains(res));

    res = "Debug: via Msg Handler\n";
    QString msgDbg(res);
    m_Logger->logMessage(QtDebugMsg, msgDbg);
    buf = m_Logger->bufferLog();
    QVERIFY(buf.contains(res));

    msg = "Test message!!";
    m_Logger->setLogMode(mafLogModeTestSuite);

    m_Logger->clearLogHistory();
    m_Logger->logMessage(QtDebugMsg, msg);
    buf = m_Logger->bufferLog();
    res = TEST_SUITE_LOG_PREFIX;
    QVERIFY(buf.contains(res));

    m_Logger->clearLogHistory();
    m_Logger->logMessage(QtWarningMsg, msg);
    buf = m_Logger->bufferLog();
    res = TEST_SUITE_LOG_PREFIX;
    QVERIFY(buf.contains(res));

    m_Logger->setLogMode(mafLogModeAllMessages);

    m_Logger->clearLogHistory();
    m_Logger->logMessage(QtCriticalMsg, msg);
    buf = m_Logger->bufferLog();
    res = "Critical: ";
    QVERIFY(buf.contains(res));

    m_Logger->clearLogHistory();
    m_Logger->logMessage(QtFatalMsg, msg);
    buf = m_Logger->bufferLog();
    res = "Fatal: ";
    QVERIFY(buf.contains(res));
}

void mafLoggerBufferTest::clearLogHistoryTest() {
    QString msg("Test for mafLoggerBuffer");
    m_Logger->logMessage(QtDebugMsg, msg);

    m_Logger->clearLogHistory();
    QString buf = m_Logger->bufferLog();

    QVERIFY(buf.isEmpty());
}

void mafLoggerBufferTest::equalTest() {
    m_Logger->clearLogHistory();

    QString msg("Test Message!!");
    mafLoggerBuffer *logger = mafNEW(mafCore::mafLoggerBuffer);

    logger->logMessage(QtDebugMsg, msg);
    m_Logger->logMessage(QtDebugMsg, msg);

    QVERIFY(m_Logger->isEqual(logger));
    mafDEL(logger);
}

#include "mafLoggerBufferTest.moc"

