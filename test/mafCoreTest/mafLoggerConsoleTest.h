/*
 *  mafLoggerConsoleTest.cpp
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafLoggerConsole.h>

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
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// create new object and check that is not NULL test case.
    void mafLoggerConsoleConstructorTest();

    /// log a string into the standard output (console)
    void logMesageTest();

private:
    mafLoggerConsole *m_Logger; ///< Testing variable.
};
