/*
 *  mafPointTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 13/12/11.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

using namespace mafResources;

void mafPointTest::initTestCase() {
    m_ObjTestVar = new mafResources::mafPoint();
}

/// Cleanup test variables memory allocation.
void mafPointTest::cleanupTestCase() {
    m_ObjTestVar->release();
}

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

#include "mafPointTest.moc"
