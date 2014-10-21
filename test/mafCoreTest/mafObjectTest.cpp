/*
 *  mafObjectTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 27/03/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

using namespace mafCore;

void mafObjectTest::initTestCase() {
    m_Memento = NULL;
    m_ObjTestVar = mafNEW(mafCore::mafObject);
}

void mafObjectTest::cleanupTestCase() {
    if(m_Memento != NULL) {
         mafDEL(m_Memento);
    }
    mafDEL(m_ObjTestVar);
}

void mafObjectTest::mafObjectConstructorTest() {
    //! <snippet>
    mafObject *obj = mafNEW(mafCore::mafObject);
    //! </snippet>
    QVERIFY(obj != NULL);

    QVariantMap *dic = obj->dictionary();
    QVERIFY(dic != NULL);

    mafDEL(obj);
}

void mafObjectTest::isObjectValidTest() {
    QVERIFY(m_ObjTestVar->isObjectValid() == true);
}

void mafObjectTest::createMementoTest() {
    m_ObjTestVar->setObjectName("Test Label");
    m_Memento = m_ObjTestVar->createMemento();
    QVERIFY(m_Memento != NULL);
}

void mafObjectTest::setMementoTest() {
    m_ObjTestVar->setObjectName("Bad name");
    //! <snippet>
    QString test_name = "Test Label";
    m_ObjTestVar->setMemento(m_Memento);
    //! </snippet>
    QString name = m_ObjTestVar->objectName();
    QCOMPARE(name, test_name);
}

//-------------------------------------------------------------------------
/**
 Class name: testVisitorCustom
 Define a custom visitor for testing 'acceptVisitor' method.
 */
class testVisitorCustom : public mafVisitor {
    Q_OBJECT
public:
    testVisitorCustom(const QString code_location = "");
    /*virtual*/ void visit(mafObjectBase *obj) {m_LabelVisited = obj->objectName();}

    /// Return m_LabelVisited's value
    const QString labelVisited() {return m_LabelVisited;}
private:
    QString m_LabelVisited; ///< Test var.
};

testVisitorCustom::testVisitorCustom(const QString code_location) : mafVisitor(code_location), m_LabelVisited("") {
}
//-------------------------------------------------------------------------

void mafObjectTest::acceptVisitorTest() {
    QString test_name = "Test Visitor Label";
    m_ObjTestVar->setObjectName(test_name);

    testVisitorCustom *vc = mafNEW(testVisitorCustom);

    m_ObjTestVar->acceptVisitor(vc);

    QCOMPARE(m_ObjTestVar->objectName(), vc->labelVisited());

    delete vc;
}

void mafObjectTest::addTagTest() {
    // Add a new tag -> return true.
    QVERIFY(m_ObjTestVar->addTag("maf 3"));
    // Add an existing tag -> return false (not added to the list).
    QVERIFY(m_ObjTestVar->addTag("maf 3") == false);

    QVERIFY(m_ObjTestVar->addTag("mafObject"));
    QVERIFY(m_ObjTestVar->addTag("maf framework"));
    QVERIFY(m_ObjTestVar->addTag("Test"));
    int s = m_ObjTestVar->tagList()->size();
    QVERIFY(s == 4);
}

void mafObjectTest::removeTagTest() {
    QVERIFY(m_ObjTestVar->removeTag("maf 3"));
    QVERIFY(!m_ObjTestVar->removeTag("not existing tag"));
}

void mafObjectTest::filterTagTest() {
    //! <snippet>
    QVariantList filteredList;
    m_ObjTestVar->filterTags("maf", &filteredList);
    int n = filteredList.count();
    //! </snippet>
    QVERIFY(n == 2);

    m_ObjTestVar->filterTags("bad", &filteredList);
    n = filteredList.count();
    QVERIFY(n == 0);
}

void mafObjectTest::addScriptTest() {
    // Add a new script -> return true.
    QVariantHash *dic = m_ObjTestVar->scriptDictionary();
    dic->insert(mafScriptKey, QVariant("#import system;"));
    QVERIFY(m_ObjTestVar->addScript(dic));

    // Add an existing script -> return false (not added to the list).
    QVERIFY(m_ObjTestVar->addScript(dic) == false);

    dic->insert(mafScriptKey, QVariant("print 2+3;"));
    QVERIFY(m_ObjTestVar->addScript(dic));

    int s = m_ObjTestVar->scriptList()->size();
    QVERIFY(s == 2);
    delete dic;
}

void mafObjectTest::removeScriptTest() {
    QVariantHash *dic = m_ObjTestVar->scriptDictionary();
    dic->insert(mafScriptKey, QVariant("#import system;"));

    QVERIFY(m_ObjTestVar->removeScript(dic));
    dic->insert(mafScriptKey, QVariant("not existing script"));
    QVERIFY(!m_ObjTestVar->removeScript(dic));

    int s = m_ObjTestVar->scriptList()->size();
    QVERIFY(s == 1);

    QVERIFY(m_ObjTestVar->removeScript(0));
    delete dic;
}

void mafObjectTest::isEqualTest() {
    // Create a new object
    mafObject *obj = mafNEW(mafCore::mafObject);

    // Extract from the test var mafObject its memento
    if(m_Memento != NULL) {
        mafDEL(m_Memento);
    }
    m_Memento = m_ObjTestVar->createMemento();
    // Assign it to the new one object so they become equals.
    // (the 'deep_memento flag at true to be copied also the hash value)
    obj->setMemento(m_Memento, true);

    QVERIFY(m_ObjTestVar->isEqual(obj));

    mafDEL(obj);
}

void mafObjectTest::setDictionaryTest() {
    // Create a new object
    mafObject *obj = mafNEW(mafCore::mafObject);

    // @@@@@@@@@@@ TO BE IMPLEMENTED!! @@@@@@@@@@
    mafDEL(obj);
}

void mafObjectTest::removeDictionaryTest() {
    // Create a new object
    mafObject *obj = mafNEW(mafCore::mafObject);

    // @@@@@@@@@@@ TO BE IMPLEMENTED!! @@@@@@@@@@

    mafDEL(obj);
}

#include "mafObjectTest.moc"

