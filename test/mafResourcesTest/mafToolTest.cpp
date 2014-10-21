/*
 *  mafToolTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 19/12/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"


using namespace mafCore;
using namespace mafResources;

void mafToolTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    mafResourcesRegistration::registerResourcesObjects();
    m_Tool = mafNEW(mafResources::mafTool);
}

void mafToolTest::cleanupTestCase() {
    mafMessageHandler::instance()->shutdown();
    mafDEL(m_Tool);
}

void mafToolTest::mafToolAllocationTest() {
    QVERIFY(m_Tool != NULL);
}

#include "mafToolTest.moc"

