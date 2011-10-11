/*
 *  mafLoggerSQLITETest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 07/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafLoggerSQLITE.h>

using namespace mafCore;

/**
 Class name: mafLoggerSQLITETest
 This class implements the test suite for mafLoggerSQLITE.
 */

//! <title>
//mafLoggerSQLITE
//! </title>
//! <description>
// mafLoggerSQLITE defines the MAF3 logging class that will store messages into a SQLITE DB.
// It override "virtual void loggedMessage(QtMsgType type, QString &msg)" used to write the given
// message into the DB.
//! </description>

class mafLoggerSQLITETest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Logger);
    }

    /// create new object and check that is not NULL test case.
    void mafLoggerSQLITEConstructorTest();

    /// test for the clearBuffer method.
    void clearLogHistoryTest();

    /// log a string into the log file.
    void logMessageTest();

private:
    mafLoggerSQLITE *m_Logger; ///< Test variable
};

void mafLoggerSQLITETest::mafLoggerSQLITEConstructorTest() {
    QString logName("log_");
    logName.append(QDateTime::currentDateTime().toString("yyyy_MMM_dd_hh_mm"));
//    logName.append(".db");
    
    m_Logger = mafNEW(mafCore::mafLoggerSQLITE);
    QVERIFY(m_Logger != NULL);

    QString res = m_Logger->lastLogFile();
    QVERIFY(res.contains(logName));
}

void mafLoggerSQLITETest::logMessageTest() {
    QString msg("Test for mafLoggerSQLITE");
    m_Logger->logMessage(QtDebugMsg, msg);

    msg = "Warning log message....";
    m_Logger->logMessage(QtWarningMsg, msg);
}

void mafLoggerSQLITETest::clearLogHistoryTest() {
    QString oldLog = m_Logger->lastLogFile();
    m_Logger->clearLogHistory();
    QString res = m_Logger->lastLogFile();

    QVERIFY(res != oldLog);
}

MAF_REGISTER_TEST(mafLoggerSQLITETest);
#include "mafLoggerSQLITETest.moc"
