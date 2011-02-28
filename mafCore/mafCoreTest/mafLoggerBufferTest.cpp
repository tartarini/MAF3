/*
 *  mafLoggerBufferTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafLoggerBuffer.h>

using namespace mafCore;

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

private slots:
    /// Initialize test variables
    void initTestCase() {
        //! <snippet>
        m_Logger = mafNEW(mafCore::mafLoggerBuffer);
        //! </snippet>
    }


    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        //! <snippet>
        mafDEL(m_Logger);
        //! </snippet>
    }

    /// create new object and check that is not NULL test case.
    void mafLoggerBufferConstructorTest();

    /// log a string into the buffer
    void logMessageTest();

    /// test for the clearBuffer method.
    void clearLogHistoryTest();

private:
    mafLoggerBuffer *m_Logger; ///< Testing variable.
    mafLogger *m_DefaultLogger;
};

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
//    QCOMPARE(buf, res);

    res = ("Warning: Test for mafLoggerBuffer\n");
    m_Logger->logMessage(QtWarningMsg, msg);
    buf = m_Logger->bufferLog();
    QVERIFY(buf.contains(res));
//    QCOMPARE(buf, res);

    res = ("Debug: via Msg Handler\n");
    QString msgDbg(res);
    m_Logger->logMessage(QtDebugMsg, msgDbg);
    buf = m_Logger->bufferLog();
    QVERIFY(buf.contains(res));
//    QCOMPARE(buf, res);
}

void mafLoggerBufferTest::clearLogHistoryTest() {
    QString msg("Test for mafLoggerBuffer");
    m_Logger->logMessage(QtDebugMsg, msg);

    m_Logger->clearLogHistory();
    QString buf = m_Logger->bufferLog();

    QVERIFY(buf.isEmpty());
}

MAF_REGISTER_TEST(mafLoggerBufferTest);
#include "mafLoggerBufferTest.moc"

