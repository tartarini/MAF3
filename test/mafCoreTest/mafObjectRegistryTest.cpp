/*
 *  mafObjectRegistryTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafObject.h>
#include <mafVisitor.h>

using namespace mafCore;

//-------------------------------------------------------------------------
/**
 Class name: testVisitorCounter
 Define a custom visitor.
 */
class testVisitorCounter : public mafVisitor {
    Q_OBJECT
public:
    testVisitorCounter(const QString code_location = "");
    /*virtual*/ void visit(mafObjectBase *obj) {++m_VisitedObjectCount;}

    /// Return m_LabelVisited's value
    const int visitedObjectCount() {return m_VisitedObjectCount;}
private:
    int m_VisitedObjectCount; ///< Test var.
};

testVisitorCounter::testVisitorCounter(const QString code_location) : mafVisitor(code_location), m_VisitedObjectCount(0) {
}
//-------------------------------------------------------------------------


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

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        m_Registry = mafObjectRegistry::instance();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
    }

    /// create new object and check that is not NULL test case.
    void mafObjectRegistryConstructorTest();

    /// test the live objects presence.
    void liveObjectsTest();

    /// test the method for finding an object from its hash code
    void objectFromHashTest();

    /// Test the apply visitor method.
    void applyVisitorToObjectListThreadedTest();

private:
    mafObjectRegistry *m_Registry; ///< Test var.
};

void mafObjectRegistryTest::mafObjectRegistryConstructorTest() {
    QVERIFY(m_Registry != NULL);
}

void mafObjectRegistryTest::liveObjectsTest() {
    //! <snippet>
    QObjectList liveObjects;
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

void mafObjectRegistryTest::applyVisitorToObjectListThreadedTest() {
    mafObjectBase *obj1 = mafNEW(mafCore::mafObjectBase);
    mafObjectBase *obj2 = mafNEW(mafCore::mafObjectBase);
    mafObjectBase *obj3 = mafNEW(mafCore::mafObjectBase);
    mafObjectsList objList;
    objList << obj1;
    objList << obj2;
    objList << obj3;

    testVisitorCounter *v = new testVisitorCounter;
    m_Registry->applyVisitorToObjectListThreaded(v, &objList);

    int n = v->visitedObjectCount();
    QVERIFY(n == 3);
    
    mafDEL(obj1);
    mafDEL(obj2);
    mafDEL(obj3);
    delete v;
}

MAF_REGISTER_TEST(mafObjectRegistryTest);
#include "mafObjectRegistryTest.moc"

