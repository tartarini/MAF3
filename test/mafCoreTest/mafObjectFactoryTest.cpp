/*
 *  mafObjectFactoryTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchion 22/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

using namespace mafCore;

void mafObjectFactoryTest::initTestCase() {
}

void mafObjectFactoryTest::cleanupTestCase() {
}

void mafObjectFactoryTest::registerObjectTest() {    
    mafUnregisterObject(mafCore::mafObjectBase);
    // Not registered object should not be present in factory.
    bool res = mafObjectFactory::instance()->isObjectRegistered("mafCore::mafObjectBase");

    QVERIFY(res == false);

    // Register mafObjectBase.
    //! <snippet>
    mafRegisterObject(mafCore::mafObjectBase);
    //! </snippet>
    // Now mafObjectBase is present into the factory.
    //! <snippet>
    res = mafObjectFactory::instance()->isObjectRegistered("mafCore::mafObjectBase");
    //! </snippet>
    QVERIFY(res == true);

    // Register qt Object
    mafRegisterQtObject(QObject)
    res = mafObjectFactory::instance()->isQtObjectRegistered("QObject");
    QVERIFY(res == true);
}

void mafObjectFactoryTest::instantiateObjectTest() {
    // registered object.
    mafObjectBase *obj_base = mafNEW(mafCore::mafObjectBase);
    QVERIFY(obj_base != NULL);
    mafDEL(obj_base);
}

void mafObjectFactoryTest::instantiateObjectBaseTest() {
    mafRegisterObject(mafCore::mafObject);
    mafObjectBase *obj = mafNEWFromString("mafCore::mafObject");
    QVERIFY(obj != NULL);
    QString cn = obj->metaObject()->className();
    QVERIFY(cn == "mafCore::mafObject");
    mafDEL(obj);
}

void mafObjectFactoryTest::instantiateQObjectTest() {
    mafRegisterQtObject(QObject)
    QObject *obj = mafNEWQt(QObject);
    QVERIFY(obj != NULL);
    delete obj;
}

void mafObjectFactoryTest::instantiateQObjectFromStringTest() {
    mafRegisterQtObject(QObject);
    QObject *obj = mafNEWQtFromString("QObject");
    QVERIFY(obj != NULL);
    QString cn = obj->metaObject()->className();
    QVERIFY(cn == "QObject");
    delete obj;
}

void mafObjectFactoryTest::unregisterObjectTest() {
    mafUnregisterObject(mafCore::mafObjectBase);
    bool res = mafObjectFactory::instance()->isObjectRegistered("mafCore::mafObjectBase");
    QVERIFY(res == false);
}

void mafObjectFactoryTest::instantiateSmartObjectTest() {
    // Register again mafObjectBase (previous test case unregistered it).
    mafRegisterObject(mafCore::mafObjectBase);
    mafSmartPointer<mafObjectBase> obj = mafCreateSmartObject(mafCore::mafObjectBase);
    QVERIFY(obj.isNull() == false);
}

void mafObjectFactoryTest::bindObjectToIconTest() {
    // Bind an object type with a file name
    QString objectType = "mafTestType";
    QString testFileName = "testFileName";

    mafBindObjectToIcon(objectType, testFileName);

    // Verify if the bind has worked, getting the file name form object type
    QString returnFileName = mafIconFromObjectType(objectType);

    QVERIFY(testFileName.compare(returnFileName) == 0);
}

#include "mafObjectFactoryTest.moc"
