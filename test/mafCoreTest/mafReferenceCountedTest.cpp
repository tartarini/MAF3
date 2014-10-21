/*
 *  mafReferenceCountedTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 13/12/11.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafReferenceCounted.h>

#include "mafCoreTestList.h"

void mafReferenceCountedTest::initTestCase() {
    m_ObjTestVar = new mafCore::mafReferenceCounted();
}

void mafReferenceCountedTest::cleanupTestCase() {
    m_ObjTestVar->release();
}

void mafReferenceCountedTest::objectAllocationTest() {
    m_ObjTestVar->retain();
    m_ObjTestVar->retain();

    int rc = m_ObjTestVar->referenceCount();
    QVERIFY(rc == 3);

    m_ObjTestVar->release();
    m_ObjTestVar->release();
}

#include "mafReferenceCountedTest.moc"

