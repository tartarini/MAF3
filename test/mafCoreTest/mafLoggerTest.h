/*
 *  mafLoggerTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafLogger.h>

// forward declaration
class testLoggerCustom;

/**
 Class name: mafLoggerTest
 This class implements the test suite for mafLogger.
 */
class mafLoggerTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    ///!brief Initialize test variables
    void initTestCase();

    ///!brief Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// create new object and check that is not NULL test case.
    void mafLoggerConstructorTest();

    /// Test the logging modality switch.
    void setLogModeTest();

    /// Test the logger enable functionality.
    void setEnabledTest();

    /// Test the logging.
    void logMessageTest();

    /// Test isEqual function.
    void isEqualTest();

private:
    testLoggerCustom *m_Logger; ///< Test variable
};
