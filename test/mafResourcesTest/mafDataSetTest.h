/*
 *  mafDataSetTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafDataSet.h>
#include <mafProxy.h>
#include <mafEventBusManager.h>
#include <mafDataBoundaryAlgorithm.h>

//forward classes
class testExternalDataType;
class testExternalBoundary;
class testDataBoundaryCustom;
class testDataSetObserver;

/**
 Class name: mafDataSetTest
 This class implements the test suite for mafDataSet.
 */

//! <title>
//mafDataSet
//! </title>
//! <description>
//mafDataSet is the base resource class for MAF3.
//It provides basic API to set input and get output from a mafResource.
//! </description>
class mafDataSetTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafDataSet allocation test case.
    void mafDataSetAllocationTest();
    /// Test the external data value assignment.
    void mafDataSetValueTest();
    /// Boundary algorithm test.
    void mafDataSetBoundaryTest();

private:
    mafResources::mafDataSet *m_DataSet; ///< Test var.
    testDataSetObserver *m_Observer; ///< Test observer
};
