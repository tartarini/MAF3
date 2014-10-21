/*
 *  mafLogicLightTest.cpp
 *  mafLogicTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 21/04/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafApplicationLogicTestList.h"

using namespace mafCore;
using namespace mafApplicationLogic;

void mafLogicLightTest::initTestCase() {
    m_LogicLight = mafNEW(mafApplicationLogic::mafLogicLight);
}

/// Cleanup tes variables memory allocation.
void mafLogicLightTest::cleanupTestCase() {
    mafDEL(m_LogicLight);
}

void mafLogicLightTest::mafLogicLightAllocationTest() {
    QVERIFY(m_LogicLight != NULL);
}

void mafLogicLightTest::initializeTest() {
    bool result = m_LogicLight->initialize();
    QVERIFY(result);
}

#include "mafLogicLightTest.moc"

