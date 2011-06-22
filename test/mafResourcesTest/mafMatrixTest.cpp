/*
 *  mafMatrixTest.cpp
 *  mafMatrixsTest
 *
 *  Created by Daniele Giunchi on 15/06/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafResourcesDefinitions.h>
#include <mafMatrix.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

/**
 Class name: mafMatrixTest
 This class implements the test suite for mafMatrix.
 */

//! <title>
//mafMatrix
//! </title>
//! <description>
//mafMatrix is the class that wrap third party library matrix.
//! </description>

class mafMatrixTest: public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafEventBusManager::instance();
        m_Matrix = new mafMatrix();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        delete m_Matrix;
        mafEventBusManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafMatrix allocation test case.
    void mafMatrixAllocationTest();
    
    /// mafMatrix clone test
    void mafMatrixCloneTest();
    
    /// mafMatrix clone test
    void mafMatrixElementAccessors();
    
    /// identity test
    void mafMatrixIdentityTest();
    
    /// test matrix multiplication and assignment
    void mafMatrixMultiplicationAndAssignmentTest();
    
    /// test rawData matrix extraction
    void mafMatrixRawDataExtractionTest();

private:
    mafMatrix *m_Matrix; ///< Test var.
};

void mafMatrixTest::mafMatrixAllocationTest() {
    QVERIFY(m_Matrix != NULL);
}

void mafMatrixTest::mafMatrixCloneTest() {
    mafMatrix *clonedMat = m_Matrix->clone();
    QVERIFY(clonedMat != NULL);
    QVERIFY(m_Matrix->isEqual(*clonedMat));
    delete clonedMat;
}

void mafMatrixTest::mafMatrixElementAccessors() {
    double valueToCheck = 5.36;
    m_Matrix->setElement(2, 3, valueToCheck);
    QVERIFY(valueToCheck == m_Matrix->element(2,3));
}

void mafMatrixTest::mafMatrixIdentityTest() {
    m_Matrix->setIdentity();
    
    mafMatrix identity;
    identity.setElement(0, 0, 1.0); identity.setElement(0, 1, 0.0); identity.setElement(0, 2, 0.0); identity.setElement(0, 3, 0.0);
    identity.setElement(1, 0, 0.0); identity.setElement(1, 1, 1.0); identity.setElement(1, 2, 0.0); identity.setElement(1, 3, 0.0);
    identity.setElement(2, 0, 0.0); identity.setElement(2, 1, 0.0); identity.setElement(2, 2, 1.0); identity.setElement(2, 3, 0.0);
    identity.setElement(3, 0, 0.0); identity.setElement(3, 1, 0.0); identity.setElement(3, 2, 0.0); identity.setElement(3, 3, 1.0);
    
    QVERIFY(m_Matrix->isEqual(identity));
}

void mafMatrixTest::mafMatrixMultiplicationAndAssignmentTest() {
    mafMatrix identity;
    identity.setIdentity();

    mafMatrix first;
    first.setElement(0, 0, 2.0); first.setElement(0, 1, 2.0); first.setElement(0, 2, 0.0); first.setElement(0, 3, 2.0);
    first.setElement(1, 0, 2.0); first.setElement(1, 1, 2.0); first.setElement(1, 2, 2.0); first.setElement(1, 3, 2.0);
    first.setElement(2, 0, 0.0); first.setElement(2, 1, 2.0); first.setElement(2, 2, 2.0); first.setElement(2, 3, 2.0);
    first.setElement(3, 0, 2.0); first.setElement(3, 1, 0.0); first.setElement(3, 2, 2.0); first.setElement(3, 3, 2.0);
    
    mafMatrix second;
    second.setElement(0, 0, 0.0); second.setElement(0, 1, 0.5); second.setElement(0, 2, -0.5);second.setElement(0, 3, 0.0);
    second.setElement(1, 0, 0.0); second.setElement(1, 1, 0.5); second.setElement(1, 2, 0.0); second.setElement(1, 3, -0.5);
    second.setElement(2, 0, -0.5);second.setElement(2, 1, 0.5); second.setElement(2, 2, 0.0); second.setElement(2, 3, 0.0);
    second.setElement(3, 0, 0.5); second.setElement(3, 1, -1.0);second.setElement(3, 2, 0.5); second.setElement(3, 3, 0.5);
    
    mafMatrix result;
    result = first * second;
    QVERIFY(result.isEqual(identity));

}

void mafMatrixTest::mafMatrixRawDataExtractionTest() {
    // | 2.0 2.0 0.0 2.0 |
    // | 3.0 2.0 2.0 2.0 |
    // | 0.0 2.0 2.0 5.0 |
    // | 1.0 0.0 2.0 2.0 |
    mafMatrix matrtix;
    matrtix.setElement(0, 0, 2.0); matrtix.setElement(0, 1, 2.0); matrtix.setElement(0, 2, 0.0); matrtix.setElement(0, 3, 2.0);
    matrtix.setElement(1, 0, 3.0); matrtix.setElement(1, 1, 2.0); matrtix.setElement(1, 2, 2.0); matrtix.setElement(1, 3, 2.0);
    matrtix.setElement(2, 0, 0.0); matrtix.setElement(2, 1, 2.0); matrtix.setElement(2, 2, 2.0); matrtix.setElement(2, 3, 5.0);
    matrtix.setElement(3, 0, 1.0); matrtix.setElement(3, 1, 0.0); matrtix.setElement(3, 2, 2.0); matrtix.setElement(3, 3, 2.0);
    
    double *val = matrtix.rawData();
    bool ok(false);
    ok = val[0] == 2.0;
    QVERIFY(ok);
    ok = val[4] == 3.0;
    QVERIFY(ok);
    ok = val[11] == 5.0;
    QVERIFY(ok);
    ok = val[12] == 1.0;
    QVERIFY(ok);
    ok = val[14] == 2.0;
    QVERIFY(ok);
}

MAF_REGISTER_TEST(mafMatrixTest);
#include "mafMatrixTest.moc"
