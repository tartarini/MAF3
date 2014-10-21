/*
 *  mafLoggerBufferTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/04/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafLoggerBuffer.h>

/**
 Class name: mafLoggerBufferTest
 This class implements the test suite for mafLoggerBuffer.
 */

//! <title>
//mafLoggerBuffer
//! </title>
//! <description>
//mafLoggerBuffer defines the MAF3 logging class that will store messages into a buffer that
//can be queried later or linked to a UI Widget. It defines the variable QString m_BufferLog
//containing all the logged messages for a specific session.
//It also redifine "virtual void loggedMessage(QtMsgType type, QString &msg)" used to log
// the given message to the buffer.
//! </description>

class mafLoggerBufferTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();


    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// create new object and check that is not NULL test case.
    void mafLoggerBufferConstructorTest();

    /// log a string into the buffer
    void logMessageTest();

    /// test for the clearBuffer method.
    void clearLogHistoryTest();

    /// test equal.
    void equalTest();

private:
    mafLoggerBuffer *m_Logger; ///< Testing variable.
    mafLogger *m_DefaultLogger;
};
