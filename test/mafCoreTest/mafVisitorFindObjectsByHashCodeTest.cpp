/*
 *  mafVisitorFindObjectsByHashCode.cpp
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 27/01/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

using namespace mafCore;

void mafVisitorFindObjectsByHashCodeTest::initTestCase() {
    m_ObjTestVar1 = mafNEW(mafCore::mafObject);
    m_ObjTestVar2 = mafNEW(mafCore::mafObject);
}

void mafVisitorFindObjectsByHashCodeTest::cleanupTestCase() {
    mafDEL(m_ObjTestVar1);
    mafDEL(m_ObjTestVar2);
}

void mafVisitorFindObjectsByHashCodeTest::visitTest() {
    //! <snippet>
    const QString &p = m_ObjTestVar1->objectHash();
    mafVisitorFindObjectsByHashCode *visitorTest = new mafVisitorFindObjectsByHashCode(&p, mafCodeLocation);
    //! </snippet>
    QVERIFY(visitorTest != NULL);

    //clear list
    visitorTest->objectsList()->clear();

    //each object that must be visited, must call accept visitor, this
    //can be very usefull in every mafObject graph structure or array
    m_ObjTestVar1->acceptVisitor(visitorTest);
    m_ObjTestVar2->acceptVisitor(visitorTest);

    //in this case only m_ObjTestVar1 will be added to the list because
    //contain the correct hash code
    QVERIFY(visitorTest->objectsList()->count() == 1);
    QVERIFY(m_ObjTestVar1->isEqual(visitorTest->objectsList()->at(0)));

    //test to check an empty list
    visitorTest->objectsList()->clear();
    m_ObjTestVar2->acceptVisitor(visitorTest);

    QVERIFY(visitorTest->objectsList()->count() == 0);

    mafDEL(visitorTest);
}

#include "mafVisitorFindObjectsByHashCodeTest.moc"


