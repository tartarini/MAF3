/*
 *  mafToolHandlerTest.cpp
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
#include <mafToolHandler.h>
#include <mafTool.h>


using namespace mafCore;
using namespace mafResources;

/**
 Class name: mafToolHandlerTest
 This class implements the test suite for mafToolHandler.
 */
class mafToolHandlerTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        m_Handler = mafNEW(mafResources::mafToolHandler);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Handler);
        mafMessageHandler::instance()->shutdown();
    }

    /// Allocation test case.
    void mafToolHandlerAllocationTest();

    /// Add and remove test
    void mafToolHandlerAddRemoveTest();

private:
    mafToolHandler *m_Handler; ///< Test Var.
};


void mafToolHandlerTest::mafToolHandlerAllocationTest() {
    QVERIFY(m_Handler != NULL);
}

void mafToolHandlerTest::mafToolHandlerAddRemoveTest() {
    QList<mafTool*> *list = m_Handler->toolList();
    int num = list->count();

    // Empty list at begin...
    QVERIFY(num == 0);

    // Add a tool to the handler...
    mafTool *t = mafNEW(mafResources::mafTool);
    m_Handler->addTool(t);
    t->release();

    num = list->count();
    QVERIFY(num == 1);

    // Remove the previously added tool...
    m_Handler->removeTool(t);
    num = list->count();
    QVERIFY(num == 0);

    // Add two new tools...
    t = mafNEW(mafResources::mafTool);
    m_Handler->addTool(t);
    t->release();

    t = mafNEW(mafResources::mafTool);
    m_Handler->addTool(t);
    t->release();

    // Remove all the handled tools...
    m_Handler->removeAllTools();

    num = list->count();
    QVERIFY(num == 0);
}

MAF_REGISTER_TEST(mafToolHandlerTest);
#include "mafToolHandlerTest.moc"

