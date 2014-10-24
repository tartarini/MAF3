/*
 *  mafInterpolatorNearestTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafResourcesRegistration.h>
#include <mafCoreSingletons.h>
#include <mafInterpolatorNearest.h>
#include <mafDataSet.h>

/**
 Class name: mafInterpolatorNearestTest
 This class implements the test suite for mafInterpolatorNearest.
 */

//! <title>
//mafInterpolatorNearest
//! </title>
//! <description>
//mafInterpolatorNearest provides the mechanism to return the element nearest the the given timestamp.
//First element or last one are returned for timestamps outside the definition range of the collection.
//! </description>

class mafInterpolatorNearestTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafInterpolatorNearest allocation test case.
    void mafInterpolatorNearestAllocationTest();
    /// Test the interpolation strategy.
    void mafInterpolationNearestStrategyTest();
    /// Test the interpolation empty collection.
    void mafInterpolationNearestEmptyCollTest();

private:
    mafResources::mafInterpolator *m_Interpolator; ///< Test var.
    mafResources::mafDataSetMap m_ItemMap; ///< Test data collection.
};
