/*
 *  mafPipeVisualCompoundTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 08/11/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

using namespace mafResources;

void mafPipeVisualCompoundTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    m_VisualPipe = mafNEW(mafResources::mafPipeVisualCompound);
}

void mafPipeVisualCompoundTest::cleanupTestCase() {
    mafDEL(m_VisualPipe);
    mafMessageHandler::instance()->shutdown();
}

void mafPipeVisualCompoundTest::mafPipeVisualCompoundAllocationTest() {
    QVERIFY(m_VisualPipe != NULL);
}

void mafPipeVisualCompoundTest::mafPipeVisualCompoundUpdateTest() {
    m_VisualPipe->updatePipe(1);
}

#include "mafPipeVisualCompoundTest.moc"
