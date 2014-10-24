/*
 *  mafPipeDataTest.h
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
#include <mafPipeData.h>
#include <mafResourcesRegistration.h>
#include <mafVME.h>
#include <mafDataSet.h>

//forward declarations
class  testDataPipeCustom;

/**
 Class name: mafPipeDataTest
 This class implements the test suite for mafPipeData.
 */
class mafPipeDataTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafPipeData allocation test case.
    void mafPipeDataAllocationTest();
    /// Test the creation and update methods..
    void mafPipeDataCreationAndUpdateTest();

    /// Test the data pipe decoration mechanism.
    void decorateTest();

    /// Test add and remove input
    void addRemoveInputTest();

    /// Test add and remove input
    void outputTest();

private:
    testDataPipeCustom *m_DataPipe; ///< Test var.
    mafResources::mafVME *m_Vme; ///< vme assigned to the data pipe
};
