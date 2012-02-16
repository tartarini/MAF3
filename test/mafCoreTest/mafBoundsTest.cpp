/*
 *  mafBoundsTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 13/12/11.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafBounds.h>
#include <mafPoint.h>

using namespace mafCore;

/**
 Class name: mafBoundsTest
 This class implements the test suite for mafBounds.
 */

//! <title>
//mafBounds
//! </title>
//! <description>
//mafBounds defines the MAF3 base object for 3D bounds.
//! </description>

class mafBoundsTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        m_ObjTestVar = new mafCore::mafBounds();
        
        double a[6] = {-2,2,-2,2,-2,2};
        double b[6] = {-1,3,-1,3,-1,3};
        m_ObjTestA = new mafCore::mafBounds(a); 
        m_ObjTestB = new mafCore::mafBounds(b);

    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        m_ObjTestVar->release();
        
        m_ObjTestA->release();
        m_ObjTestB->release();
    }

    /// Test the constructors
    void objectAllocationTest();

    /// Test the bound values.
    void boundsTest();
    
    void equalTest();
    
    /// unite test.
    void uniteBoundsTest();
    
    /// intersection test.
    void intersectBoundsTest();
    
    /// point inside/outside bounds test
    void pointInsideOutsideBounds();
    
private:
    mafBounds *m_ObjTestVar; ///< Test variable
    mafBounds *m_ObjTestA; ///< Test variable
    mafBounds *m_ObjTestB; ///< Test variable
};

void mafBoundsTest::objectAllocationTest() {
    m_ObjTestVar->retain();
    m_ObjTestVar->retain();

    int rc = m_ObjTestVar->referenceCount();
    QVERIFY(rc == 3);

    m_ObjTestVar->release();
    m_ObjTestVar->release();
    
    bool res = mafEquals(0, m_ObjTestVar->xMin());
    QVERIFY(res);
    
    res = mafEquals(-1, m_ObjTestVar->xMax());
    QVERIFY(res);
}

void mafBoundsTest::boundsTest() {
    double b[6] = {0, 2, 0, 3, 0, 4};
    mafBounds *bounds = new mafBounds(b);
    QVERIFY(bounds->isObjectValid());
    mafDEL(bounds);
}

void mafBoundsTest::equalTest() {
    QVERIFY(!(*m_ObjTestA == *m_ObjTestB));
}


void mafBoundsTest::uniteBoundsTest() {
    mafBounds *outputTest = new mafCore::mafBounds();
    m_ObjTestA->unite(*m_ObjTestB, *outputTest);
    
    /// check if output bounds are correct
    double bc[6] = {-2.,3.,-2.,3.,-2.,3.};
    mafBounds *checked = new mafCore::mafBounds(bc);
    QVERIFY(*checked == *outputTest);
    
    mafDEL(outputTest);
    mafDEL(checked);
    
}

void mafBoundsTest::intersectBoundsTest() {
    mafBounds *outputTest = new mafCore::mafBounds();
    m_ObjTestA->intersect(*m_ObjTestB, *outputTest);
    
    /// check if output bounds are correct
    double bc[6] = {-1.,2.,-1.,2.,-1.,2.};
    mafBounds *checked = new mafCore::mafBounds(bc);
    QVERIFY(*checked == *outputTest);
    
    mafDEL(outputTest);
    mafDEL(checked);    
}

void mafBoundsTest::pointInsideOutsideBounds() {
    double o[3] = {0.,0.,0.};
    mafPoint *origin = new mafCore::mafPoint(o);
    bool result = m_ObjTestA->isPointInBounds(origin);
    QVERIFY(result);
    mafDEL(origin);
    
    double c[3] = {10.,10.,10.};
    mafPoint *custom = new mafCore::mafPoint(c);
    result = m_ObjTestA->isPointInBounds(custom);
    QVERIFY(!result);
}

MAF_REGISTER_TEST(mafBoundsTest);
#include "mafBoundsTest.moc"
