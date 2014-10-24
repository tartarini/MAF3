/*
 *  mafPipeTest.h
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
#include <mafPipe.h>
#include <mafVME.h>
#include <mafDataSet.h>

//forward declarations
class testPipeCustom;

/**
 Class name: mafPipeTest
 This class implements the test suite for mafPipe.
 */
class mafPipeTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafPipe allocation test case.
    void mafPipeAllocationTest();
    /// Test the creation and update methods.
    void mafPipeCreationAndUpdateTest();
    /// Test the input management of the mafPipe
    void inputManagementTest();
    /// Test the parameter initialization through hash
    void setParameterHashTest();

private:
    testPipeCustom *m_Pipe; ///< Test var.
};
