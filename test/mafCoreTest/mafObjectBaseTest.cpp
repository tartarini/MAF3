/*
 *  mafObjectBaseTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 22/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

using namespace mafCore;

class testObjectBaseCustom : public mafObjectBase {
    Q_OBJECT
    Q_PROPERTY(bool customBool READ customBool)

public:
    /// Object constructor.
    testObjectBaseCustom() : m_CustomVar(true) {};
    
    /// Return the internal boolean variable.
    bool customBool() const;

protected:
private:
    bool m_CustomVar; ///< test var.
};

inline bool testObjectBaseCustom::customBool() const {
    return m_CustomVar;
}

void mafObjectBaseTest::initTestCase() {
    m_ObjTestVar = mafNEW(mafCore::mafObjectBase);
}

void mafObjectBaseTest::cleanupTestCase() {
    m_ObjTestVar->release();
}

void mafObjectBaseTest::objectIdTest() {
    //! <snippet>
    mafId id = m_ObjTestVar->objectId();
    //! </snippet>
    QVERIFY(id != -1);

    m_ObjTestVar->retain();
    m_ObjTestVar->retain();

    m_ObjTestVar->release();
    m_ObjTestVar->release();
}

//void mafObjectBaseTest::createObjectHashTest() {
//    QByteArray res_ba("f\022\225\311\313\371\326\262\366B\204\024PJ\215\356\323\002\006A");
//    QString hash_key = "test string";
//    m_ObjTestVar->createHashCode(hash_key);
//    QByteArray ba = m_ObjTestVar->objectHash();
//    QVERIFY(ba == res_ba);
//}

void mafObjectBaseTest::isEqualTest() {
    testObjectBaseCustom *obj = new testObjectBaseCustom();
    bool res = m_ObjTestVar->isEqual(obj);
    QVERIFY(!res);
    
    testObjectBaseCustom *obj2 = new testObjectBaseCustom();
    res = obj->isEqual(obj2);
    QVERIFY(res);

    obj2->release();
    obj->release();
}

void mafObjectBaseTest::modifiedTest() {
    m_ObjTestVar->setModified();
    bool res = m_ObjTestVar->modified();
    QVERIFY(res);

    m_ObjTestVar->setModified(false);
    res = m_ObjTestVar->modified();
    QVERIFY(!res);

    m_ObjTestVar->description();
}

#include "mafObjectBaseTest.moc"

