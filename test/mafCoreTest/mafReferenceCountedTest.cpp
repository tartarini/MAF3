/*
 *  mafReferenceCountedTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 13/12/11.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafReferenceCounted.h>

using namespace mafCore;

/**
 Class name: mafReferenceCountedTest
 This class implements the test suite for mafReferenceCounted.
 */

//! <title>
//mafReferenceCounted
//! </title>
//! <description>
//mafReferenceCounted defines the MAF3 base object.
//Its main function is to provide a memory management system based on reference count mechanism.
//! </description>

class mafReferenceCountedTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        m_ObjTestVar = new mafCore::mafReferenceCounted();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        m_ObjTestVar->release();
    }

    /// Test the ID generation for the mafObjectBase
    void objectAllocationTest();

private:
    mafReferenceCounted *m_ObjTestVar; ///< Test variable
};

void mafReferenceCountedTest::objectAllocationTest() {
    m_ObjTestVar->retain();
    m_ObjTestVar->retain();

    int rc = m_ObjTestVar->referenceCount();
    QVERIFY(rc == 3);

    m_ObjTestVar->release();
    m_ObjTestVar->release();
}

MAF_REGISTER_TEST(mafReferenceCountedTest);
#include "mafReferenceCountedTest.moc"

