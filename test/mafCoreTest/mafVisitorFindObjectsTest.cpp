/*
 *  mafVisitorFindObjectsTest.cpp
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 27/04/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafVisitorFindObjects.h>

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

/**
 Class name: mafVisitorFindObjectsTest
 This class implements the test suite for mafVisitorFindObjects.
 */
class mafVisitorFindObjectsTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_VisitorFindObjects = mafNEW(testVisitorFindObjectsCustom);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_VisitorFindObjects);
    }

    /// mafVisitorFindObjects allocation test case.
    void mafVisitorFindObjectsAllocationTest();
    /// Test the creation and update methods..
    void mafVisitorFindObjectsObjectListTest();

private:
    testVisitorFindObjectsCustom *m_VisitorFindObjects; ///< Test var.
};

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

MAF_REGISTER_TEST(mafVisitorFindObjectsTest);
#include "mafVisitorFindObjectsTest.moc"

