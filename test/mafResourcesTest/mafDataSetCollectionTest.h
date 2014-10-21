/*
 *  mafDataSetCollectionTest.cpp
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
#include <mafProxy.h>
#include <mafMatrix4x4.h>
#include <mafDataSetCollection.h>
#include <mafDataSet.h>

//forward declaration
class testPoseObserver;
class testCustomExternalData;
class testCustomAnotherExternalData;

/**
 Class name: mafDataSetCollectionTest
 This class implements the test suite for mafDataSetCollection.
 */

 //! <title>
//mafDataSetCollection
//! </title>
//! <description>
//mafDataSetCollection is the class representing the time varying data for MAF3.
//! </description>
class mafDataSetCollectionTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafDataSetCollection allocation test case.
    void collectionAllocationTest();
    /// Test orientation and pose for mafDataSetCollection
    void collectionPoseMatrixTest();
    /// Test the pose matrix synchronization
    void collectionSynchronizePoseTest();
    /// Test new item insertion
    void collectionInsertItemTest();
    /// Test the setDataSet method
    void collectionDataSetTest();
    /// Test the remove item from the collection
    void collectionRemoveItemTest();

private:
    mafResources::mafDataSetCollection *m_Collection; ///< Test var.
    testPoseObserver *m_PoseObserver; ///< Modify dataset collection's pose observer;
};
