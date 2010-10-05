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

/**
 Class name: mafCoreRegistrationTest
 This class implements the test suite for mafCoreRegistration.
 */
class mafCoreRegistrationTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafCoreRegistration::registerCoreObjects();
        m_Object = NULL;
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Object);
    }

    /// Call the registration for the objects present in mafCore module.
    void mafCoreRegistrationRegisterObjectsTest();

private:
    mafObject *m_Object; ///< Test var.
};

void mafCoreRegistrationTest::mafCoreRegistrationRegisterObjectsTest() {
    m_Object = (mafObject *)mafNEWFromString("mafCore::mafObject");
    QVERIFY(m_Object != NULL);
}

MAF_REGISTER_TEST(mafCoreRegistrationTest);
#include "mafCoreRegistrationTest.moc"

