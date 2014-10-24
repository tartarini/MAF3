/*
 *  mafMementoHierarchyTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafHierarchy.h>
#include <mafMementoHierarchy.h>

/**
 Class name: mafMementoHierarchyTest
 This class implements the test suite for mafMementoHierarchy.
 */
class mafMementoHierarchyTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafMementoHierarchy allocation test case.
    void mafMementoHierarchyDefaultAllocationTest();

    /// mafMementoHierarchy traverse a more complex hierarchy
    void mafMementoHierarchyTraverseTreeTest();

    
private:
};
