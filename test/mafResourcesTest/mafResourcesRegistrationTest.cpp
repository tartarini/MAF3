/*
 *  mafResourcesRegistrationTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 22/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

using namespace mafCore;
using namespace mafResources;

void mafResourcesRegistrationTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
}

void mafResourcesRegistrationTest::cleanupTestCase() {
    mafMessageHandler::instance()->shutdown();
}

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

#include "mafResourcesRegistrationTest.moc"
