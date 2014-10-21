/*
 *  mafDataBoundaryAlgorithmTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafResourcesRegistration.h>
#include <mafCoreSingletons.h>
#include <mafDataBoundaryAlgorithm.h>

//forward declaration
class  testDataBoundaryAlgorithmCustom;

/**
 Class name: mafDataBoundaryAlgorithmTest
 This class implements the test suite for mafInterpolator.
 */
class mafDataBoundaryAlgorithmTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// testDataBoundaryAlgorithmCustom allocation test case.
    void mafBoundaryAlgorithmAllocationTest();
    /// Test the boundary algorithm strategy.
    void mafBoundaryAlgorithmStrategyTest();

private:
    testDataBoundaryAlgorithmCustom *m_BoundaryAlgorithm; ///< Test var.
};
