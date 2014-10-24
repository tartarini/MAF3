/*
 *  mafVisitorTest.cpp
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

//-------------------------------------------------------------------------
/**
 Class name: mafVisitorCustom
 Define a custom visitor for testing 'acceptVisitor' method.
 */
class testVisitorImplement : public mafVisitor {
    Q_OBJECT
public:
    testVisitorImplement(const QString code_location = "");
    /*virtual*/ void visit(mafObjectBase *obj) {m_LabelVisited = obj->objectName();}

    /// Return m_LabelVisited's value
    const QString labelVisited() {return m_LabelVisited;}
private:
    QString m_LabelVisited; ///< Test var.
};

testVisitorImplement::testVisitorImplement(const QString code_location) : mafVisitor(code_location), m_LabelVisited("") {
}
//-------------------------------------------------------------------------


void mafVisitorTest::initTestCase() {
    m_Object = mafNEW(mafCore::mafObject);
    m_Object->setObjectName("Test");
    m_VisitorImplement = mafNEW(testVisitorImplement);
}

void mafVisitorTest::cleanupTestCase() {
    mafDEL(m_Object);
    mafDEL(m_VisitorImplement);
}

void mafVisitorTest::visitorTest() {
    m_Object->acceptVisitor(m_VisitorImplement);
    QString res("Test");
    QCOMPARE(res, m_Object->objectName());
}

#include "mafVisitorTest.moc"

