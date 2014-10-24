/*
 *  mafToolTest.h
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
#include <mafTool.h>
#include <mafVME.h>

/**
 Class name: mafToolTest
 This class implements the test suite for mafTool.
 */
class mafToolTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// Allocation test case.
    void mafToolAllocationTest();

private:
    mafResources::mafTool *m_Tool; ///< Test var.
};


