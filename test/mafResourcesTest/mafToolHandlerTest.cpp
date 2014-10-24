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

#include "mafResourcesTestList.h"

using namespace mafCore;
using namespace mafResources;

void mafToolHandlerTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    mafResourcesRegistration::registerResourcesObjects();
    m_Handler = mafNEW(mafResources::mafToolHandler);
}

void mafToolHandlerTest::cleanupTestCase() {
    mafDEL(m_Handler);
    mafMessageHandler::instance()->shutdown();
}


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

#include "mafToolHandlerTest.moc"

