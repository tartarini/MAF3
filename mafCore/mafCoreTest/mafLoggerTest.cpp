/*
 *  mafLoggerTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafLogger.h>

using namespace mafCore;

//-------------------------------------------------------------------------
/**
 Class name: testLoggerCustom
 Define a custom Logger.
 */
class testLoggerCustom : public mafLogger {
public:
    /// Object constructor.
    testLoggerCustom(const mafString code_location = "");

    /// Clear all the logged messages until now.
    /*virtual*/ void clearLogHistory();

    /// Return the log buffer
    mafString buffer() {return m_Buffer;}

protected:
    /// Virtual method implementation of logMessage.
    /** You have to redefine this method in your sublass and put here your strategy to store the log message into the specific output stream media (file, db, buffer, console, ...).*/
    /*virtual*/ void loggedMessage(const mafMsgType type, const mafString &msg);

private:
    mafString m_Buffer; ///< Test var
};

testLoggerCustom::testLoggerCustom(const mafString code_location) : mafLogger(code_location) {
}

void testLoggerCustom::clearLogHistory() {
    m_Buffer = "";
}

void testLoggerCustom::loggedMessage(const mafMsgType type, const mafString &msg) {
    Q_UNUSED(type);
    m_Buffer = msg;
}

//-------------------------------------------------------------------------


/**
 Class name: mafLoggerTest
 This class implements the test suite for mafLogger.
 */
class mafLoggerTest : public QObject {
    Q_OBJECT

private slots:
    ///!brief Initialize test variables
    void initTestCase() {
        m_Logger = mafNEW(testLoggerCustom);
    }

    ///!brief Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Logger);
    }

    /// create new object and check that is not NULL test case.
    void mafLoggerConstructorTest();

    /// Test the logging modality switch.
    void setLogModeTest();

    /// Test the logger enable functionality.
    void setEnabledTest();

    /// Test the logging.
    void logMessageTest();

    /// Test isEqual function.
    void isEqualTest();

private:
    testLoggerCustom *m_Logger; ///< Test variable
};

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
    mafString msg("Test");
    m_Logger->logMessage(mafMsgTypeDebug, msg);
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

MAF_REGISTER_TEST(mafLoggerTest);
#include "mafLoggerTest.moc"

