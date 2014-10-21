/*
 *  mafMementoDataSetCollectionTest.cpp
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
#include <mafDataSetCollection.h>
#include <mafProxy.h>
#include <mafExternalDataCodec.h>
#include <mafProxy.h>
#include <mafVME.h>
#include <mafDataSet.h>
#include <mafMemento.h>

//! <title>
//mafMementoDataSetCollection
//! </title>
//! <description>
//mafMementoDataSetCollection aims to store a mafDataSetCollection state implementing a sort
//of undo mechanism for the object's state. This is used to restore
// a previous stored DataSet state (undo mechanism or serialization purposes).
//! </description>


/**
 Class name: mafMementoDataSetCollectionTest
 This class implements the test suite for mafMementoDataSetCollection.
 */
class mafMementoDataSetCollectionTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafMementoDataSetCollection allocation test case.
    void mafMementoDataSeCollectiontDefaultAllocationTest();
    /// mafMementoDataSetCollection test case.
    void mafMementoDataSetCollectionAllocationTest();

private:
    mafResources::mafDataSet *m_DataSet; ///< Test var.
};
