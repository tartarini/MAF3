/*
 *  mafObjectBaseTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafObjectBase.h>

using namespace mafCore;

/**
 Class name: mafObjectBaseTest
 This class implements the test suite for mafObjectBase.
 */

//! <title>
//mafObjectBase
//! </title>
//! <description>
//mafObjectBase defines the MAF3 base object.
//Its main function is to provide a unique ID (mafId) to each mafObject and to add the object to the mafObjectRegistry.
//! </description>

class mafObjectBaseTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_ObjTestVar = mafNEW(mafCore::mafObjectBase);
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        m_ObjTestVar->deleteObject();
    }

    /// Test the ID generation for the mafObjectBase
    void objectIdTest();
    /// Test the creation of the objectHash.
//    void createObjectHashTest();
    /// Test the isEqual method.
    void isEqualTest();

private:
    mafObjectBase *m_ObjTestVar; ///< Test variable
};

void mafObjectBaseTest::objectIdTest() {
    //! <snippet>
    mafId id = m_ObjTestVar->objectId();
    //! </snippet>
    QVERIFY(id != -1);

    m_ObjTestVar->ref();
    m_ObjTestVar->ref();

    m_ObjTestVar->deleteObject();
    m_ObjTestVar->deleteObject();
}

//void mafObjectBaseTest::createObjectHashTest() {
//    mafByteArray res_ba("f\022\225\311\313\371\326\262\366B\204\024PJ\215\356\323\002\006A");
//    mafString hash_key = "test string";
//    m_ObjTestVar->createHashCode(hash_key);
//    mafByteArray ba = m_ObjTestVar->objectHash();
//    QVERIFY(ba == res_ba);
//}

void mafObjectBaseTest::isEqualTest() {
//    mafString hash_key = "test string";
//    m_ObjTestVar->createHashCode(hash_key);
//
//    mafObjectBase *obj2 = mafNEW(mafCore::mafObjectBase);
//    obj2->createHashCode(hash_key);
//
//    QVERIFY(m_ObjTestVar->isEqual(obj2));
//    bool result = *m_ObjTestVar == *obj2;
//    QVERIFY(result);
//
//    delete obj2;
}

MAF_REGISTER_TEST(mafObjectBaseTest);
#include "mafObjectBaseTest.moc"

