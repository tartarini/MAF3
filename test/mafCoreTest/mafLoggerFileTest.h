/*
 *  mafLoggerTest.cpp
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafLoggerFile.h>

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

class mafLoggerFileTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// create new object and check that is not NULL test case.
    void mafLoggerFileConstructorTest();

    /// test for the clearBuffer method.
    void clearLogHistoryTest();

    /// log a string into the log file.
    void logMessageTest();

private:
    mafLoggerFile *m_Logger; ///< Test variable
    mafLogger *m_DefaultLogger;
};
