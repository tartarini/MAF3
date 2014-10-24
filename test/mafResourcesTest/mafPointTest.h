/*
 *  mafPointTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafPoint.h>

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
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// Test the ID generation for the mafObjectBase
    void objectAllocationTest();
    
    /// test point equality
    void pointsEqualTest();

private:
    mafResources::mafPoint *m_ObjTestVar; ///< Test variable
};
