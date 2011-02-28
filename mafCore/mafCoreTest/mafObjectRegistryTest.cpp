/*
 *  mafObjectRegistryTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafObject.h>

using namespace mafCore;

/**
 Class name: mafObjectRegistryTest
 This class implements the test suite for mafObjectRegistry.
 */

//! <title>
//mafObjectRegistry
//! </title>
//! <description>
//mafObjectRegistry is a singleton which provides the place where allocated objects
//are monitored. It allows to monitor when, where and who allocated the object.
//
//When the constructor of a mafObjectBase is called, the object is added to the
//mafObjectRegistry instance. Similarly, when an object is deleted
//"mafObjectRegistry::instance()->removeObject(m_ObjectId)" is called and it will be
//removed from the registry. This class is useful for monitoring memory allocation,
//detect memory leaks and for garbage collection functionality.
//
//It also define the method "void dumpLiveObjects()" which dumps into default output
//stream all live objects still allocated and not deleted.
//! </description>

class mafObjectRegistryTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_Registry = mafObjectRegistry::instance();
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
    }

    /// create new object and check that is not NULL test case.
    void mafObjectRegistryConstructorTest();
    /// test the live objects presence.
    void liveObjectsTest();

    /// test the method for finding an object from its hash code
    void objectFromHashTest();

private:
    mafObjectRegistry *m_Registry; ///< Test var.
};

void mafObjectRegistryTest::mafObjectRegistryConstructorTest() {
    QVERIFY(m_Registry != NULL);
}

void mafObjectRegistryTest::liveObjectsTest() {
    //! <snippet>
    mafObjectsList liveObjects;
    m_Registry->liveObjects(&liveObjects);
    //! </snippet>
    int n0 = liveObjects.count();

    mafObject *obj1 = mafNEW(mafCore::mafObject);
    m_Registry->liveObjects(&liveObjects);

    int n1 = liveObjects.count();

    QVERIFY(n1 - n0 >= 1);

    mafObject *obj2 = mafNEW(mafCore::mafObject);
    m_Registry->liveObjects(&liveObjects);
    int n2 = liveObjects.count();
    QVERIFY(n2 - n0 >= 2);

    mafDEL(obj1);
    m_Registry->liveObjects(&liveObjects);
    int n3 = liveObjects.count();
    QVERIFY(n3 - n0 >= 1);


    mafDEL(obj2);
    m_Registry->liveObjects(&liveObjects);
    int n4 = liveObjects.count();
    QVERIFY(n4 == n0);
}

void mafObjectRegistryTest::objectFromHashTest() {
    mafObject *obj1 = mafNEW(mafCore::mafObject);
    QString validHash = obj1->objectHash();

     // Search an object by the given hash string.
    mafObjectBase *checkObject = m_Registry->objectFromHash(validHash);
    QVERIFY(obj1->isEqual(checkObject));

    // Now try to search objects with an invalid hash (NULL will be returned).
    checkObject = m_Registry->objectFromHash("");
    QVERIFY(checkObject == NULL);

    mafDEL(obj1);
}

MAF_REGISTER_TEST(mafObjectRegistryTest);
#include "mafObjectRegistryTest.moc"

