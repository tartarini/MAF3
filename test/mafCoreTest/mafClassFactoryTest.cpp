/*
 *  mafClassFactoryTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 27/03/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"
#include <mafClassFactory.h>
#include <mafObject.h>

void mafClassFactoryTest::initTestCase() {
}

void mafClassFactoryTest::cleanupTestCase() {
}

void mafClassFactoryTest::mafClassFactoryAllocationTest() {
    //Create an instance of a mafObjectBase
    mafObjectBase *obj = m_ClassFactory.make(mafCodeLocation);
    QVERIFY(obj != NULL);
    mafDEL(obj);

    //Create an instance of a qObject
    QObject *qObj = m_ClassFactory.make();
    QVERIFY(qObj != NULL);
    delete qObj;
}

#include "mafClassFactoryTest.moc"
