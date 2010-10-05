/*
 *  mafMessageHandlerTest.cpp
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
#include <mafLoggerConsole.h>

using namespace mafCore;

/**
 Class name: mafMessageHandlerTest
 This class implements the test suite for mafMessageHandler.
 */

//! <title>
//mafMessageHandler
//! </title>
//! <description>
//mafMessageHandler is a singleton which provides the dispatching of logging messages.
//The message is forwarded to the current active logger only if it is enabled and the
//message type has not been filtered.
//For example: if the logger is in the mode of enable logging only for Critical messages,
//a message is forwarded to it only if it is critical and so on.
//! </description>

class mafMessageHandlerTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_Handler = mafMessageHandler::instance();
        m_Handler->installMessageHandler();
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
    }

    /// create new object and check that is not NULL test case.
    void mafMessageHandlerConstructorTest();

    /// Test the active logging assignment.
    void setActiveloggerTest();

private:
    mafMessageHandler *m_Handler; ///< Testing variable.
};

void mafMessageHandlerTest::mafMessageHandlerConstructorTest() {
    QVERIFY(m_Handler != NULL);
}

void mafMessageHandlerTest::setActiveloggerTest() {
    mafRegisterObject(mafLoggerConsole);
    mafLogger *logger = new mafLoggerConsole();
    m_Handler->setActiveLogger(logger);

    mafString cn = logger->metaObject()->className();
    mafString res = "mafCore::mafLoggerConsole";

    // Default logger is mafLoggerConsole.
    QCOMPARE(cn, res);
    mafDEL(logger);
    m_Handler->setDeafultLoggerAsActive();
}

MAF_REGISTER_TEST(mafMessageHandlerTest);
#include "mafMessageHandlerTest.moc"


