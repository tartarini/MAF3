/*
 *  mafPipeVisualCompoundTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 08/11/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafPipeVisualCompound.h>
#include <mafVME.h>

using namespace mafResources;

/**
 Class name: mafPipeVisualCompoundTest
 This class implements the test suite for mafPipeVisualCompound.
 */
class mafPipeVisualCompoundTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        m_VisualPipe = mafNEW(mafResources::mafPipeVisualCompound);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_VisualPipe);
        mafMessageHandler::instance()->shutdown();
    }

    /// mafPipeVisual allocation test case.
    void mafPipeVisualCompoundAllocationTest();
    /// Test the creation and update methods..
    void mafPipeVisualCompoundUpdateTest();
	
private:
    mafPipeVisualCompound *m_VisualPipe; ///< Test var.
};

void mafPipeVisualCompoundTest::mafPipeVisualCompoundAllocationTest() {
    QVERIFY(m_VisualPipe != NULL);
}

void mafPipeVisualCompoundTest::mafPipeVisualCompoundUpdateTest() {
    m_VisualPipe->updatePipe(1);
}

MAF_REGISTER_TEST(mafPipeVisualCompoundTest);
#include "mafPipeVisualCompoundTest.moc"
