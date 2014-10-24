/*
 *  mafLoggerTest.cpp
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

//-------------------------------------------------------------------------
/**
 Class name: testLoggerCustom
 Define a custom Logger.
 */
class testLoggerCustom : public mafLogger {
    Q_OBJECT
public:
    /// Object constructor.
    testLoggerCustom(const QString code_location = "");

    /// Clear all the logged messages until now.
    /*virtual*/ void clearLogHistory();

    /// Return the log buffer
    QString buffer() {return m_Buffer;}

protected:
    /// Virtual method implementation of logMessage.
    /** You have to redefine this method in your sublass and put here your strategy to store the log message into the specific output stream media (file, db, buffer, console, ...).*/
    /*virtual*/ void loggedMessage(const QtMsgType type, const QString &msg);

private:
    QString m_Buffer; ///< Test var
};

testLoggerCustom::testLoggerCustom(const QString code_location) : mafLogger(code_location) {
}

void testLoggerCustom::clearLogHistory() {
    m_Buffer = "";
}

void testLoggerCustom::loggedMessage(const QtMsgType type, const QString &msg) {
    Q_UNUSED(type);
    m_Buffer = msg;
}

//-------------------------------------------------------------------------

void mafLoggerTest::initTestCase() {
        m_Logger = mafNEW(testLoggerCustom);
    }

void mafLoggerTest::cleanupTestCase() {
    mafDEL(m_Logger);
}

void mafLoggerTest::mafLoggerConstructorTest() {
    QVERIFY(m_Logger != NULL);
}

void mafLoggerTest::setLogModeTest() {
    // default log mode is mafLogModeAllMessages.
    mafLogMode lm = m_Logger->logMode();
    QVERIFY(lm == mafLogModeAllMessages);

    m_Logger->setLogMode(mafLogModeCustom);
    lm = m_Logger->logMode();
    QVERIFY(lm == mafLogModeCustom);
}

void mafLoggerTest::setEnabledTest() {
    QVERIFY(m_Logger->enabled());

    m_Logger->setEnabled(false);
    QVERIFY(!m_Logger->enabled());
}

void mafLoggerTest::logMessageTest() {
    QString msg("Test");
    m_Logger->logMessage(QtDebugMsg, msg);
    QCOMPARE(m_Logger->buffer(), msg);

    m_Logger->clearLogHistory();
    QVERIFY(m_Logger->buffer().length() == 0);
}

void mafLoggerTest::isEqualTest() {
    testLoggerCustom *myLogger = mafNEW(testLoggerCustom);
    m_Logger->setEnabled(true);
    QVERIFY(!myLogger->isEqual(m_Logger));

    myLogger->setLogMode(mafLogModeCustom);
    QVERIFY(myLogger->isEqual(m_Logger));
    mafDEL(myLogger);
}

#include "mafLoggerTest.moc"

