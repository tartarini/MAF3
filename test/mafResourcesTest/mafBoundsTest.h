/*
 *  mafBoundsTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafBounds.h>
#include <mafMatrix4x4.h>

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
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// Test the constructors
    void objectAllocationTest();

    /// Test the bound values.
    void boundsTest();
    
    /// test is two mafBounds objects are equal.
    void equalTest();
    
    /// unite test.
    void uniteBoundsTest();
    
    /// intersection test.
    void intersectBoundsTest();
    
    /// point inside/outside bounds test
    void pointInsideOutsideBounds();
    
    /// unite without a pivot variable
    void autoUniteTest();

    /// Transform test.
    void transformBoundsTest();
    
private:
    mafResources::mafBounds *m_ObjTestVar; ///< Test variable
    mafResources::mafBounds *m_ObjTestA; ///< Test variable
    mafResources::mafBounds *m_ObjTestB; ///< Test variable
};
