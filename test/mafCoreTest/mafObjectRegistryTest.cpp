/*
 *  mafObjectRegistryTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 27/03/09.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

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


void mafObjectRegistryTest::initTestCase() {
    m_Registry = mafObjectRegistry::instance();
}


void mafObjectRegistryTest::cleanupTestCase() {
}

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

#include "mafObjectRegistryTest.moc"

