/*
 *  mafVisitorFindObjectsTest.cpp
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 27/04/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

using namespace mafCore;

//------------------------------------------------------------------------------------------
/**
 Class name: testVisitorFindObjectsCustom
 This class implements the visitor to be tested.
 */
class  testVisitorFindObjectsCustom : public  mafVisitorFindObjects {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafVisitorFindObjects);

public:
    /// Object constructor.
    testVisitorFindObjectsCustom(const QString code_location = "");

    /// implement visit method
    void visit(mafObjectBase *obj);
};

testVisitorFindObjectsCustom::testVisitorFindObjectsCustom(const QString code_location) : mafVisitorFindObjects(code_location) {
}

void testVisitorFindObjectsCustom::visit(mafObjectBase *obj) {
    m_ObjectsList->push_back(obj);
}

//------------------------------------------------------------------------------------------

void mafVisitorFindObjectsTest::initTestCase() {
    m_VisitorFindObjects = mafNEW(testVisitorFindObjectsCustom);
}

void mafVisitorFindObjectsTest::cleanupTestCase() {
    mafDEL(m_VisitorFindObjects);
}

void mafVisitorFindObjectsTest::mafVisitorFindObjectsAllocationTest() {
    QVERIFY(m_VisitorFindObjects != NULL);
}

void mafVisitorFindObjectsTest::mafVisitorFindObjectsObjectListTest() {
    mafObjectBase * obj1 = mafNEW(mafCore::mafObjectBase);
    mafObjectBase * obj2 = mafNEW(mafCore::mafObjectBase);

    mafObjectsList *list = m_VisitorFindObjects->objectsList();
    QVERIFY(list->count() == 0);
    m_VisitorFindObjects->visit(obj1);
    list = m_VisitorFindObjects->objectsList();
    QVERIFY(list->count() == 1);
    m_VisitorFindObjects->visit(obj2);
    list = m_VisitorFindObjects->objectsList();
    QVERIFY(list->count() == 2);

    mafDEL(obj1);
    mafDEL(obj2);
}

#include "mafVisitorFindObjectsTest.moc"

