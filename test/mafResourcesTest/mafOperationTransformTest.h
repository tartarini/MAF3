/*
 *  mafOperationTransformTest.h
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
#include <mafOperationTransform.h>
#include <mafVMEManager.h>
#include <mafVME.h>
#include <mafDataSet.h>
#include <QDebug>

/**
 Class name: mafOperationTransformTest
 This class implements the test suite for mafOperationTransform.
 */

//! <title>
//mafOperationTransform
//! </title>
//! <description>
//mafOperationTransform will move a VME modifying its pose matrix.
//! </description>

class mafOperationTransformTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// Test the operation's execution.
    void testExecute();

   /// Test Set/Get method of transform operation
    void SetGetTest();

private:
    mafResources::mafOperationTransform *m_OpTransform; ///< Transform operation.
    mafResources::mafVME *m_VME; ///< represents the vme that will be moved
    mafResources::mafVMEManager *m_VMEManager; ///< instance of mafVMEManager.
    mafResources::mafDataSet *m_DataSet;
};
