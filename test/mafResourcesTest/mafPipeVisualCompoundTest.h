/*
 *  mafPipeVisualCompoundTest.h
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
#include <mafPipeVisualCompound.h>
#include <mafVME.h>

/**
 Class name: mafPipeVisualCompoundTest
 This class implements the test suite for mafPipeVisualCompound.
 */
class mafPipeVisualCompoundTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafPipeVisual allocation test case.
    void mafPipeVisualCompoundAllocationTest();
    /// Test the creation and update methods..
    void mafPipeVisualCompoundUpdateTest();
	
private:
    mafResources::mafPipeVisualCompound *m_VisualPipe; ///< Test var.
};
