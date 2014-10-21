/*
 *  mafLoggerSQLITETest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafLoggerSQLITE.h>

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
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// create new object and check that is not NULL test case.
    void mafLoggerSQLITEConstructorTest();

    /// test for the clearBuffer method.
    void clearLogHistoryTest();

    /// log a string into the log file.
    void logMessageTest();

private:
    mafLoggerSQLITE *m_Logger; ///< Test variable
};
