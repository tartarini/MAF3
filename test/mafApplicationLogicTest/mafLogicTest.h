/*
 *  mafLogicTest.h
 *  mafLogicTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2010 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTestSuite.h"
#include <mafPluginManager.h>
#include <mafLogic.h>
#include <mafCoreSingletons.h>
#include <mafObject.h>

//forwaard declaration
class testObjectCustomPlugged;

/**
 Class name: mafLogicTest
 This class implements the test suite for mafLogic.
 */
class mafLogicTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// Check the existence of the mafLogic allocation.
    void mafLogicAllocationTest();

    /// Test the initialization of the mafLogic
    void initializeTest();

    /// Test the working directory accessors.
    void workingDirectoryTest();

    /// Test the plugObject functionality.
    void plugObjectTest();

private:
    mafApplicationLogic::mafLogic *m_Logic; ///< Test variable.
    mafResources::mafPluginManager *m_PluginManager; ///< Test var.
};
