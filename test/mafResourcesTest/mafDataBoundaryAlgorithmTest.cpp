/*
 *  mafDataBoundaryAlgorithmTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 22/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

using namespace mafCore;
using namespace mafResources;

//------------------------------------------------------------------------------------------
/**
 Class name: testDataBoundaryAlgorithmCustom
 This class implements the boundary algorithm to be tested.
 */
class  testDataBoundaryAlgorithmCustom : public  mafDataBoundaryAlgorithm {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafDataBoundaryAlgorithm);

public:
    /// Object constructor.
    testDataBoundaryAlgorithmCustom(const QString code_location = "");

    /// Algorithm that will be used to extract the boundary from the given data value and pose matrix.
    /*virtual*/ mafProxyInterface *calculateBoundary(mafProxyInterface *data, mafMatrix4x4 *matrix);

    /// Algorithm that will be used to extract the boundary from the given bounds and pose matrix.
    /*virtual*/ mafCore::mafProxyInterface *calculateBoundary(double bounds[6], mafMatrix4x4 *matrix = NULL);

    /// Return bounds of the bounding box.
    /*virtual*/ void bounds(double bounds[6]);

    /// Return the m_BoundaryItem.
    QString boundaryItem();

private:
    QString m_BoundaryItem; ///< Test Var.
};

testDataBoundaryAlgorithmCustom::testDataBoundaryAlgorithmCustom(const QString code_location) : mafDataBoundaryAlgorithm(code_location), m_BoundaryItem("") {
}

QString testDataBoundaryAlgorithmCustom::boundaryItem() {
    return m_BoundaryItem;
}

mafProxyInterface *testDataBoundaryAlgorithmCustom::calculateBoundary(mafProxyInterface *data, mafMatrix4x4 *matrix) {
    Q_UNUSED(data);
    Q_UNUSED(matrix);

    m_BoundaryItem = "BoundaryCalculated";
    return NULL;
}

mafCore::mafProxyInterface *testDataBoundaryAlgorithmCustom::calculateBoundary(double bounds[6], mafMatrix4x4 *matrix){
    Q_UNUSED(bounds);
    Q_UNUSED(matrix);

    m_BoundaryItem = "BoundaryCalculated";
    return NULL;
}

void testDataBoundaryAlgorithmCustom::bounds(double bounds[6]) {
    Q_UNUSED(bounds);
}
//------------------------------------------------------------------------------------------

void mafDataBoundaryAlgorithmTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    m_BoundaryAlgorithm = mafNEW(testDataBoundaryAlgorithmCustom);
}

void mafDataBoundaryAlgorithmTest::cleanupTestCase() {
    mafDEL(m_BoundaryAlgorithm);
    mafMessageHandler::instance()->shutdown();
}

void mafDataBoundaryAlgorithmTest::mafBoundaryAlgorithmAllocationTest() {
    QVERIFY(m_BoundaryAlgorithm != NULL);
}

void mafDataBoundaryAlgorithmTest::mafBoundaryAlgorithmStrategyTest() {
    QString res("BoundaryCalculated");
    m_BoundaryAlgorithm->calculateBoundary(NULL);
    QCOMPARE(m_BoundaryAlgorithm->boundaryItem(), res);
}

#include "mafDataBoundaryAlgorithmTest.moc"
