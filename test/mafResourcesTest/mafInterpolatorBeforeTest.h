/*
 *  mafInterpolatorBeforeTest.cpp
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
#include <mafInterpolatorBefore.h>
#include <mafDataSet.h>

/**
 Class name: mafInterpolatorBeforeTest
 This class implements the test suite for mafInterpolatorBefore.
 */

 //! <title>
//mafInterpolatorBefore
//! </title>
//! <description>
//mafInterpolatorBefore provides the mechanism to return the element suddenly before the
//given timestamp. If no elements are defined before the given timestamp, NULL is returned.
//! </description>
class mafInterpolatorBeforeTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafInterpolatorBefore allocation test case.
    void mafInterpolatorBeforeAllocationTest();
    /// Test the interpolation strategy.
    void mafInterpolationBeforeStrategyTest();

private:
    mafResources::mafInterpolator *m_Interpolator; ///< Test var.
    mafResources::mafDataSetMap m_ItemMap; ///< Test data collection.
};
