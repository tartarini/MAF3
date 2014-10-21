/*
 *  mafMementoResourceTest.h
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafMementoResource.h>

//! <title>
//mafMementoResource
//! </title>
//! <description>
//mafMementoResource defines the MAF3 base resource for storing
//a mafResource state. This can be used to restore a previous
//stored resource state (undo mechanism or serialization purposes).
//! </description>

/**
 Class name: mafMementoResourceTest
 This class implements the test suite for mafMementoResource.
 */
class mafMementoResourceTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafMementoResource allocation test case.
    void mafMementoResourceDefaultAllocationTest();
    /// mafMementoResource allocation test case.
    void mafMementoResourceCustomAllocationTest();

private:
    mafResources::mafMementoResource *m_MementoResource; ///< Test var.
};
