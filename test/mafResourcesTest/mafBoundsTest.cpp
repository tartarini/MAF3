/*
 *  mafBoundsTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 13/12/11.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

using namespace mafResources;

void mafBoundsTest::initTestCase() {
	mafMessageHandler::instance()->installMessageHandler();
    m_ObjTestVar = new mafResources::mafBounds();
    
    double a[6] = {-2,2,-2,2,-2,2};
    double b[6] = {-1,3,-1,3,-1,3};
    m_ObjTestA = new mafResources::mafBounds(a); 
    m_ObjTestB = new mafResources::mafBounds(b);

}

void mafBoundsTest::cleanupTestCase() {
    m_ObjTestVar->release();
    
    m_ObjTestA->release();
    m_ObjTestB->release();
	mafMessageHandler::instance()->shutdown();
}

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
    QVERIFY(mafEquals(bounds->xMin(), 0));
    QVERIFY(mafEquals(bounds->xMax(), 2));
    QVERIFY(mafEquals(bounds->yMin(), 0));
    QVERIFY(mafEquals(bounds->yMax(), 3));
    QVERIFY(mafEquals(bounds->zMin(), 0));
    QVERIFY(mafEquals(bounds->zMax(), 4));
    mafDEL(bounds);
}

void mafBoundsTest::equalTest() {
    QVERIFY(!(*m_ObjTestA == *m_ObjTestB));
    QVERIFY(*m_ObjTestA != *m_ObjTestB);
}


void mafBoundsTest::uniteBoundsTest() {
    m_ObjTestA->unite(*m_ObjTestB);
    
    /// check if output bounds are correct
    double bc[6] = {-2.,3.,-2.,3.,-2.,3.};
    mafBounds *checked = new mafResources::mafBounds(bc);
    QVERIFY(*checked == *m_ObjTestA);
    
    mafDEL(checked);
    
}

void mafBoundsTest::autoUniteTest() {
    mafBounds *autoTest = new mafResources::mafBounds();
    double a[6] = {-2,2,-2,2,-2,2};
    m_ObjTestA->setBounds(a);
    autoTest->setBounds(m_ObjTestA);
    autoTest->unite(*m_ObjTestB);
    
    /// check if output bounds are correct
    double bc[6] = {-2.,3.,-2.,3.,-2.,3.};
    mafBounds *checked = new mafResources::mafBounds(bc);
    QVERIFY(*checked == *autoTest);
    
    mafDEL(autoTest);
    mafDEL(checked);
    
}

void mafBoundsTest::intersectBoundsTest() {
    double a[6] = {-2,2,-2,2,-2,2};
    m_ObjTestA->setBounds(a);
    m_ObjTestA->intersect(*m_ObjTestB);
    
    /// check if output bounds are correct
    double bc[6] = {-1.,2.,-1.,2.,-1.,2.};
    mafBounds *checked = new mafResources::mafBounds(bc);
    QVERIFY(*checked == *m_ObjTestA);
    
    mafDEL(checked);    
}

void mafBoundsTest::pointInsideOutsideBounds() {
    double o[3] = {0.,0.,0.};
    mafPoint *origin = new mafResources::mafPoint(o);
    bool result = m_ObjTestA->isPointInBounds(origin);
    QVERIFY(result);
    mafDEL(origin);
    
    double c[3] = {10.,10.,10.};
    mafPoint *custom = new mafResources::mafPoint(c);
    result = m_ObjTestA->isPointInBounds(custom);
    QVERIFY(!result);
    mafDEL(custom);
}

void mafBoundsTest::transformBoundsTest() {
    // Create a test bounds
    double b[6] = {0, 1, 0, 2, 0, 4};
    mafBounds *b0 = new mafBounds(b, mafCodeLocation);

    // Create a transformation matrix.
    mafMatrix4x4 m;
    m.setToIdentity();
    m(0,3) = 3.;
	m(1,3) = 2.;
    m(2,3) = 1.;

    // Transform the bounds using the matrix.
    b0->transformBounds(&m);

    // Create the result bounds.
    double bRes[6] = {3, 4, 2, 4, 1, 5};
    mafBounds *b0Res = new mafBounds(bRes, mafCodeLocation);

    // Check that are equals.
    bool res = (*b0) == (*b0Res);
    QVERIFY(res);

    // Free the allocated memory.
    mafDEL(b0);
    mafDEL(b0Res);

    // Create a cubed bounds
    double bCube[6] = {-.5, .5, -.5, .5, -.5, .5};
    mafBounds *boundsCube = new mafBounds(bCube, mafCodeLocation);

    mafMatrix4x4 *transformMatrix = new mafMatrix4x4();
    transformMatrix->setToIdentity();
    (*transformMatrix)(0,0) = 0.707;
    (*transformMatrix)(0,1) = -.707;
    (*transformMatrix)(1,0) = .707;
    (*transformMatrix)(1,1) = .707;

    boundsCube->transformBounds(transformMatrix);
    double x_min = boundsCube->xMin();
    double x_max = boundsCube->xMax();
    double y_min = boundsCube->yMin();
    double y_max = boundsCube->yMax();
    double z_min = boundsCube->zMin();
    double z_max = boundsCube->zMax();

    mafDEL(boundsCube);
    delete transformMatrix;
    transformMatrix = NULL;
}

#include "mafBoundsTest.moc"
