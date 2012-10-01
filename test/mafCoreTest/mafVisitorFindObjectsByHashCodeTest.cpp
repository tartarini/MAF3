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

#include <mafTestSuite.h>
#include <mafObject.h>

#include <mafVisitorFindObjectsByHashCode.h>

using namespace mafCore;

//! <title>
//mafVisitorFindObjectsByHashCode
//! </title>
//! <description>
//mafVisitorFindObjectsByHashCode define a modality of visiting object checking the hash code
//giving in input from the constructor of the visitor
//! </description>

/**
 Class name: mafVisitorFindObjectsByHashCodeTest
 This class implements the test suite for mafObject.
 */
class mafVisitorFindObjectsByHashCodeTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        m_ObjTestVar1 = mafNEW(mafCore::mafObject);
        m_ObjTestVar2 = mafNEW(mafCore::mafObject);
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_ObjTestVar1);
        mafDEL(m_ObjTestVar2);
    }

    /// register new object in factory test case.
    void visitTest();

private:
    mafObject *m_ObjTestVar1; ///< Test variable
    mafObject *m_ObjTestVar2; ///< Test variable
};

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

MAF_REGISTER_TEST(mafVisitorFindObjectsByHashCodeTest);
#include "mafVisitorFindObjectsByHashCodeTest.moc"


