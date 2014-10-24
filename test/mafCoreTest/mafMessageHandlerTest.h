/*
 *  mafMessageHandlerTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
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

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();
    
    /// create new object and check that is not NULL test case.
    void mafMessageHandlerConstructorTest();

    /// Test the active logging assignment.
    void setActiveloggerTest();

private:
    mafMessageHandler *m_Handler; ///< Testing variable.
};
