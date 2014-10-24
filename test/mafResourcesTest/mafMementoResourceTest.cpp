/*
 *  mafMementoResourceTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

using namespace mafCore;
using namespace mafResources;

void mafMementoResourceTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    mafResourcesRegistration::registerResourcesObjects();
    m_MementoResource = NULL;
}

void mafMementoResourceTest::cleanupTestCase() {
    mafDEL(m_MementoResource);
    mafMessageHandler::instance()->shutdown();
}


void mafMementoResourceTest::mafMementoResourceDefaultAllocationTest() {
    mafMemento *m = (mafMemento *)mafNEWFromString("mafResources::mafMementoResource");
    QVERIFY(m != NULL);
    m_MementoResource = qobject_cast<mafMementoResource*>(m);
    QVERIFY(m_MementoResource != NULL);
}

void mafMementoResourceTest::mafMementoResourceCustomAllocationTest() {
    mafDEL(m_MementoResource);

    mafObject *obj = mafNEW(mafCore::mafObject);
    mafResourceList inputList;

    //! <snippet>
    m_MementoResource = new mafMementoResource(obj, &inputList, mafCodeLocation);
    //! </snippet>
    QVERIFY(m_MementoResource != NULL);

    mafDEL(obj);
}

#include "mafMementoResourceTest.moc"

