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

#include <QFileInfo>

using namespace mafCore;

/**
 Class name: mafLoggerFileTest
 This class implements the test suite for mafLoggerFile.
 */

//! <title>
//mafLoggerFile
//! </title>
//! <description>
//mafLoggerFile defines the MAF3 logging class that will store messages into a file
//on filesystem. It creates a temporary file into a temporary directory and redifine
//"virtual void loggedMessage(QtMsgType type, QString &msg)" used to write the given
//message into the temporary file created.
//! </description>

void mafLoggerFileTest::initTestCase() {
    //! <snippet>
    m_Logger = mafNEW(mafCore::mafLoggerFile);
    //! </snippet>
}

void mafLoggerFileTest::cleanupTestCase() {
    mafDEL(m_Logger);
}

void mafLoggerFileTest::mafLoggerFileConstructorTest() {
    QVERIFY(m_Logger != NULL);
    QString log_file = m_Logger->lastLogFile();
    QVERIFY(QFile::exists(log_file));
}

void mafLoggerFileTest::logMessageTest() {
    QString log_file = m_Logger->lastLogFile();
    QFileInfo fInfo(log_file);
    QVERIFY(fInfo.size() == 0); // Empty file at begin.

    //! <snippet>
    QString msg("Test for mafLoggerFile");
    m_Logger->logMessage(QtDebugMsg, msg);
    //! </snippet>

    msg = "Warning message";
    m_Logger->logMessage(QtWarningMsg, msg);

//    QVERIFY(fInfo.size() > 0); // Cannot be verified because the
                                 // file is flushed only when the logger is destroyed.
                                 // I checked the correctness of the file writing manually
                                 // by looking at the file when the test ends :)
}

void mafLoggerFileTest::clearLogHistoryTest() {
    QString log_file = m_Logger->lastLogFile();
    m_Logger->clearLogHistory();

    QVERIFY(!QFile::exists(log_file)); // Should not exists; after clear the old file is deleted.

    // Get the new log filename (after the clearLogHistory call
    // it has been changed)
    log_file = m_Logger->lastLogFile();
    QVERIFY(QFile::exists(log_file)); // Should exists.
}

#include "mafLoggerFileTest.moc"
