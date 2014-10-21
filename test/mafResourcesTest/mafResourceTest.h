/*
 *  mafResourceTest.h
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafMementoResource.h>
#include <mafResource.h>


/**
 Class name: mafResourceTest
 This class implements the test suite for mafResource.
 */

//! <title>
//mafResource
//! </title>
//! <description>
//mafResource is the base resource class for MAF3.
//It provides basic API to set input and get output from a mafResource.
//! </description>

class mafResourceTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafResource allocation test case.
    void mafResourceAllocationTest();
    /// Test the add input functionality
    void mafResourceAddInputTest();
    /// Test the remove input functionality
    void mafResourceRemoveInputTest();
    /// Test the createMemento functionality.
    void mafResourceCreateMementoTest();

private:
    mafResources::mafResource *m_Resource; ///< Test var.
};
