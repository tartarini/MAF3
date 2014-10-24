/*
 *  mafResourcesRegistrationTest.h
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafResourcesRegistration.h>

/**
 Class name: mafResourcesRegistrationTest
 This class implements the test suite for mafResourcesRegistration.
 */
class mafResourcesRegistrationTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafResourcesRegistration registration test case.
    void registrationAndUnregistrationTest();
};
