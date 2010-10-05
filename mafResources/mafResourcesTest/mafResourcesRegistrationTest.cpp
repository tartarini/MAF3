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
#include <mafVisualPipe.h>

using namespace mafCore;
using namespace mafResources;

//----------------------------------------------------------------------------------------------------
/**
 Class name: testVisualPipeCustomForResourcesRegistration
 This visual pipe that implements the 'acceptObject' validation function.
 */
class testVisualPipeCustomForResourcesRegistration : public mafVisualPipe {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafVisualPipe);

public:
    /// Object constructor.
    testVisualPipeCustomForResourcesRegistration(const mafString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObject *obj);

    /// Initialize and create the pipeline
    /*virtual*/ void createPipe();

public slots:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);
};

testVisualPipeCustomForResourcesRegistration::testVisualPipeCustomForResourcesRegistration(const mafString code_location) : mafVisualPipe(code_location) {
}

bool testVisualPipeCustomForResourcesRegistration::acceptObject(mafCore::mafObject *obj) {
    mafVME *vme = dynamic_cast<mafVME*>(obj);
    if(vme != NULL) {
        return true;
    }
    return false;
}

void testVisualPipeCustomForResourcesRegistration::createPipe() {

}

void testVisualPipeCustomForResourcesRegistration::updatePipe(double t) {
    Q_UNUSED(t);
}

//----------------------------------------------------------------------------------------------------

/**
 Class name: mafResourcesRegistrationTest
 This class implements the test suite for mafResourcesRegistration.
 */
class mafResourcesRegistrationTest: public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
    }

    /// mafResourcesRegistration registration test case.
    void registrationTest();
    /// mafResourcesRegistration acceptObject test case
    void acceptObjectTest();
};

void mafResourcesRegistrationTest::registrationTest() {
    // Register all the objects in the mafResources module.
    mafResourcesRegistration::registerResourcesObjects();

    // Create an instance of one of those using the mafNEWFromString macro
    mafObjectBase *obj = mafNEWFromString("mafResources::mafResource");

    // Check that the object has been created
    QVERIFY(obj != NULL);
    // free its memory.
    mafDEL(obj);
}

void mafResourcesRegistrationTest::acceptObjectTest() {
    // Register the custom visual pipe using the macro for registering itself also for binding check
    mafRegisterObjectAndAcceptBind(testVisualPipeCustomForResourcesRegistration);

    mafStringList binding_class_list;

    // Create two test objects.
    mafObject *obj = mafNEW(mafCore::mafObject);
    mafVME *vme = mafNEW(mafResources::mafVME);

    // Check that the 'obj' showld not be valid and 'vme' yes instead.
    binding_class_list = mafResourcesRegistration::acceptObject(obj);
    int num = binding_class_list.count();
    QVERIFY(num == 0);

    binding_class_list = mafResourcesRegistration::acceptObject(vme);
    num = binding_class_list.count();
    mafMsgDebug() << num;
    QVERIFY(num != 0);

    // Check that the visual pipe has been registered correctly to the object factory.
    testVisualPipeCustomForResourcesRegistration *vp = (testVisualPipeCustomForResourcesRegistration *)mafNEWFromString("testVisualPipeCustomForResourcesRegistration");
    QVERIFY(vp != NULL);

    mafDEL(obj);
    mafDEL(vme);
    mafDEL(vp);

    mafUnregisterObjectAndAcceptUnbind(testVisualPipeCustomForResourcesRegistration);
}

MAF_REGISTER_TEST(mafResourcesRegistrationTest);
#include "mafResourcesRegistrationTest.moc"
