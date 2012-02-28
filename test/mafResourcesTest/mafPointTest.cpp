/*
 *  mafPointTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 13/12/11.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafPoint.h>

using namespace mafResources;

/**
 Class name: mafPointTest
 This class implements the test suite for mafPoint.
 */

//! <title>
//mafPoint
//! </title>
//! <description>
// mafPoint defines the MAF3 base object for 3D point.
//! </description>

class mafPointTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        m_ObjTestVar = new mafResources::mafPoint();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        m_ObjTestVar->release();
    }

    /// Test the ID generation for the mafObjectBase
    void objectAllocationTest();
    
    /// test point equality
    void pointsEqualTest();

private:
    mafPoint *m_ObjTestVar; ///< Test variable
};

void mafPointTest::objectAllocationTest() {
    QVERIFY(m_ObjTestVar);
    
    m_ObjTestVar->retain();
    m_ObjTestVar->retain();

    int rc = m_ObjTestVar->referenceCount();
    QVERIFY(rc == 3);

    m_ObjTestVar->release();
    m_ObjTestVar->release();
}

void mafPointTest::pointsEqualTest() {
    mafPoint *pointA;
    mafPoint *pointB;

    pointA = new mafPoint(1.,2.,3.);
    double p[3] = {1.,2.,3.};
    pointB = new mafPoint(p);
    
    QVERIFY(!(*pointA != *pointB));
    
    
}

MAF_REGISTER_TEST(mafPointTest);
#include "mafPointTest.moc"
