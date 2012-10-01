/*
 *  mafToolTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 19/12/11.
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


using namespace mafCore;
using namespace mafResources;

/**
 Class name: mafToolTest
 This class implements the test suite for mafTool.
 */
class mafToolTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        m_Tool = mafNEW(mafResources::mafTool);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafMessageHandler::instance()->shutdown();
        mafDEL(m_Tool);
    }

    /// Allocation test case.
    void mafToolAllocationTest();

private:
    mafTool *m_Tool; ///< Test var.
};

void mafToolTest::mafToolAllocationTest() {
    QVERIFY(m_Tool != NULL);
}

MAF_REGISTER_TEST(mafToolTest);
#include "mafToolTest.moc"

