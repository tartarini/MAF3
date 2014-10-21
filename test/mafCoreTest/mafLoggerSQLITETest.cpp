/*
 *  mafLoggerSQLITETest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 07/10/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"
#include <mafLoggerSQLITE.h>

using namespace mafCore;

void mafLoggerSQLITETest::initTestCase() {
}

void mafLoggerSQLITETest::cleanupTestCase() {
    mafDEL(m_Logger);
}

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
    qDebug() << "Old Log: " << oldLog;
    QTest::qSleep(2000);
    m_Logger->clearLogHistory();
    QString res = m_Logger->lastLogFile();
    qDebug() << "New Log: " << res;
    QVERIFY(res != oldLog);
}

#include "mafLoggerSQLITETest.moc"
