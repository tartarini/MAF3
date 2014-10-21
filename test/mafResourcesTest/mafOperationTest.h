/*
 *  mafOperationTest.h
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafOperation.h>

//forward declarations
class testOperation;

/**
 Class name: mafOperationTest
 This class implements the test suite for mafOperation.
 */

//! <title>
//mafOperation
//! </title>
//! <description>
//mafOperation  provides basic API for building elaboration algorithms for mafResources.
//An operation takes as input one or more mafVMEs and generate as output a mafVME.
//The algorithm executed on the input data is provided ad mafPipeData. The mafOperation
//that manage the possibility to have the undo mechanism for the executed algorithm.
//! </description>
class mafOperationTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafOperation allocation test case.
    void mafOperationAllocationTest();

private:
    mafResources::mafOperation *m_Operation; ///< Test var.
};
