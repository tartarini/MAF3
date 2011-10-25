/*
 *  mafObjectTest.cpp
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
#include <mafMementoObject.h>
#include <mafVisitor.h>

using namespace mafCore;

//! <title>
//mafObject
//! </title>
//! <description>
//mafObject defines the basic API for objects that wants to be serialized and visited by the mafVisitor class, 
//provides undo mechanism and defines API to control the serialization of tags.
//If you want to create an object with undo state mechanism and tags serialization, it has to inherit from mafObject.
//
//It defines some virtual methods:
//"virtual mafMementoObject *createMemento()"  and "virtual void setMemento(mafMementoObject *memento)"  to implement 
//a sort of undo mechanism for the object's state creating and setting the status of the object mafMementoObject.
//"virtual bool isObjectValid() const" used in MAF3 design by contract to check object's validity. isObjectValid methods play
//an important role in checking the consistency of objects in the debug. isObjectValid is defined as a pure virtual function
//in Object class, thus it needs to be overridden in all inheriting classes.The inheriting class should perform defensive
//checks to make sure that it is in a consistent state. Also note that this method is only available in the debug build.
//"virtual void acceptVisitor(mafVisitor *v)" to allows to accept a mafVisitor which will visit the object and will be 
//executed the mafVisitor algorithm.
//It defines API to add, remove and filter tags: tags wants to define a series of strings that describe the object and
//categorize it in the cloud of MAF3 objects (like a photo tags on Flickr).
//! </description>

/**
 Class name: mafObjectTest
 This class implements the test suite for mafObject.
 */
class mafObjectTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        m_Memento = NULL;
        m_ObjTestVar = mafNEW(mafCore::mafObject);
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        if(m_Memento != NULL) {
             mafDEL(m_Memento);
        }
        mafDEL(m_ObjTestVar);
    }

    /// register new object in factory test case.
    void mafObjectConstructorTest();
    /// object validity test case.
    void isObjectValidTest();
    /// memento creation test case.
    void createMementoTest();
    /// memento assignment test case.
    void setMementoTest();
    /// accept visitor test case.
    void acceptVisitorTest();
    /// Test the Object's addTag functionality.
    void addTagTest();
    /// Test the Object's removeTag functionality.
    void removeTagTest();
    /// Test the Object's filterTag functionality.
    void filterTagTest();
    /// Test the Object's addScript functionality.
    void addScriptTest();
    /// Test the Object's removeScript functionality.
    void removeScriptTest();
    /// Test isEqual method.
    void isEqualTest();    
    /// Test setDictionary method.
    void setDictionaryTest();
    /// Test removeDictionary method.
    void removeDictionaryTest();

private:
    mafObject *m_ObjTestVar; ///< Test variable
    mafMemento *m_Memento; ///< Memento test var
};

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

MAF_REGISTER_TEST(mafObjectTest);
#include "mafObjectTest.moc"

