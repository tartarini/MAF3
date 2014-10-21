/*
 *  mafDelegateTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreRegistration.h>
#include <mafDelegate.h>

// forward declaration
class testDelegate;

/**
 Class name: mafDelegateTest
 This class implements the test suite for mafDelegate.
 */
class mafDelegateTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// create new object and check that is not NULL test case.
    void mafDelegateConstructorTest();

    /// Test the presence of method definition
    void isMethodDefinedTest();

    /// Verify that the shouldExecuteLocalCode return true by default.
    void shouldExecuteLocalCodeTest();

    /// Test the method execution.
    void executeMethodTest();

private:
    testDelegate *m_Delegate; ///< Test variable.
};
