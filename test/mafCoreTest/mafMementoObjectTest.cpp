/*
 *  mafMementoObjectTest.cpp
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

void mafMementoObjectTest::initTestCase() {
}

void mafMementoObjectTest::cleanupTestCase() {
}

void mafMementoObjectTest::mafMementoConstructorTest() {
    mafObject *obj = mafNEW(mafCore::mafObject);
    obj->setObjectName("Test memento");
    //! <snippet>
    mafMementoObject *memento = new mafMementoObject(obj, mafCodeLocation);
    //! </snippet>
    QVERIFY(memento != NULL);
    mafDEL(memento);
    mafDEL(obj);
}

#include "mafMementoObjectTest.moc"

