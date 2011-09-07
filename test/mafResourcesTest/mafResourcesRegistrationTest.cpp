/*
 *  mafResourcesRegistrationTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafResourcesRegistration.h>

using namespace mafCore;
using namespace mafResources;

/**
 Class name: mafResourcesRegistrationTest
 This class implements the test suite for mafResourcesRegistration.
 */
class mafResourcesRegistrationTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafMessageHandler::instance()->shutdown();
    }

    /// mafResourcesRegistration registration test case.
    void registrationAndUnregistrationTest();
};

void mafResourcesRegistrationTest::registrationAndUnregistrationTest() {
    // Register all the objects in the mafResources module.
    mafResourcesRegistration::registerResourcesObjects();

    // Create an instance of one of those using the mafNEWFromString macro
    mafObjectBase *obj = mafNEWFromString("mafResources::mafResource");

    // Check that the object has been created
    QVERIFY(obj != NULL);
    // free its memory.
    mafDEL(obj);

    mafResourcesRegistration::unregisterResourcesObjects();
}

MAF_REGISTER_TEST(mafResourcesRegistrationTest);
#include "mafResourcesRegistrationTest.moc"
