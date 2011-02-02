/*
 *  mafCoreRegistrationTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreRegistration.h>

using namespace mafCore;

//----------------------------------------------------------------------------------------------------
/**
 Class name: testVisualPipeCustomForResourcesRegistration
 This visual pipe that implements the 'acceptObject' validation function.
 */
class testObjectCustomForCoreRegistration : public mafObject {
    Q_OBJECT
    mafSuperclassMacro(mafCore::mafObject);

public:
    /// Object constructor.
    testObjectCustomForCoreRegistration(const mafString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);
};

testObjectCustomForCoreRegistration::testObjectCustomForCoreRegistration(const mafString code_location) : mafObject(code_location) {
}

bool testObjectCustomForCoreRegistration::acceptObject(mafCore::mafObjectBase *obj) {
    mafObject *object = dynamic_cast<mafCore::mafObject*>(obj);
    if(object != NULL) {
        const mafTagList *tagList = object->tagList();
        if(tagList->size() > 0) {
            return true;
        }
    }
    return false;
}

//----------------------------------------------------------------------------------------------------


/**
 Class name: mafCoreRegistrationTest
 This class implements the test suite for mafCoreRegistration.
 */
class mafCoreRegistrationTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafCoreRegistration::registerCoreObjects();
        m_Object = NULL;
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Object);
    }

    /// Call the registration for the objects present in mafCore module.
    void mafCoreRegistrationRegisterObjectsTest();
    /// mafCoreRegistration acceptObject test case
    void acceptObjectTest();

private:
    mafObject *m_Object; ///< Test var.
};

void mafCoreRegistrationTest::mafCoreRegistrationRegisterObjectsTest() {
    m_Object = (mafObject *)mafNEWFromString("mafCore::mafObject");
    QVERIFY(m_Object != NULL);
}

void mafCoreRegistrationTest::acceptObjectTest() {
    // Register the custom visual pipe using the macro for registering itself also for binding check
    mafRegisterObjectAndAcceptBind(testObjectCustomForCoreRegistration);

    mafStringList binding_class_list;

    // Create two test objects.
    mafObject *obj_accepted = mafNEW(mafCore::mafObject);
    obj_accepted->addTag("Test Tag");

    mafObject *obj_rejected = mafNEW(mafCore::mafObject);

    // Check that the 'obj_rejected' showld not be valid and 'obj_accepted' yes instead.
    binding_class_list = mafCoreRegistration::acceptObject(obj_rejected);
    int num = binding_class_list.count();
    QVERIFY(num == 0);

    binding_class_list = mafCoreRegistration::acceptObject(obj_accepted);
    num = binding_class_list.count();
    mafMsgDebug() << num;
    QVERIFY(num != 0);

    // Check that the test object has been registered correctly to the object factory.
    testObjectCustomForCoreRegistration *obj = (testObjectCustomForCoreRegistration *)mafNEWFromString("testObjectCustomForCoreRegistration");
    QVERIFY(obj != NULL);

    mafDEL(obj_accepted);
    mafDEL(obj_rejected);
    mafDEL(obj);

    mafUnregisterObjectAndAcceptUnbind(testObjectCustomForCoreRegistration);
}

MAF_REGISTER_TEST(mafCoreRegistrationTest);
#include "mafCoreRegistrationTest.moc"

