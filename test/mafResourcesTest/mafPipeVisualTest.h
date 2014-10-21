/*
 *  mafPipeVisualTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafPipeVisual.h>
#include <mafVME.h>
#include <mafProxy.h>

//forward declarations
class testVisualPipeCustomForVisualPipe;

/**
 Class name: mafPipeVisualTest
 This class implements the test suite for mafPipeVisual.
 */
class mafPipeVisualTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafPipeVisual allocation test case.
    void mafPipeVisualAllocationTest();
    /// Test the creation and update methods..
    void mafPipeVisualCreationAndUpdateTest();
private:
    testVisualPipeCustomForVisualPipe *m_VisualPipe; ///< Test var.
};
