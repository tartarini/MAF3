/*
 *  mafToolHandlerTest.h
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
#include <mafToolHandler.h>
#include <mafTool.h>


/**
 Class name: mafToolHandlerTest
 This class implements the test suite for mafToolHandler.
 */
class mafToolHandlerTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// Allocation test case.
    void mafToolHandlerAllocationTest();

    /// Add and remove test
    void mafToolHandlerAddRemoveTest();

private:
    mafResources::mafToolHandler *m_Handler; ///< Test Var.
};
