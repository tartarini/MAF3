/*
 *  mafCoreRegistrationTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/04/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafObject.h>

/**
 Class name: mafCoreRegistrationTest
 This class implements the test suite for mafCoreRegistration.
 */
class mafCoreRegistrationTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// Call the registration for the objects present in mafCore module.
    void mafCoreRegistrationRegisterObjectsTest();
    
    /// mafCoreRegistration acceptObject test case
    void acceptObjectTest();

private:
    mafObject *m_Object; ///< Test var.
};
