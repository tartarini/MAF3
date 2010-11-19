/*
 *  mafDataBoundaryAlgorithmTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafResourcesRegistration.h>
#include <mafCoreSingletons.h>
#include <mafDataBoundaryAlgorithm.h>

using namespace mafCore;
using namespace mafResources;

//------------------------------------------------------------------------------------------
/**
 Class name: testDataBoundaryAlgorithmCustom
 This class implements the boundary algorithm to be tested.
 */
class  testDataBoundaryAlgorithmCustom : public  mafDataBoundaryAlgorithm {
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafDataBoundaryAlgorithm);

public:
    /// Object constructor.
    testDataBoundaryAlgorithmCustom(const mafString code_location = "");

    /// Algorithm that will be used to extract the boundary from the given data value and pose matrix.
    /*virtual*/ mafContainerInterface *calculateBoundary(mafContainerInterface *data, mafPoseMatrix *matrix);

    /// Algorithm that will be used to extract the boundary from the given bounds and pose matrix.
    /*virtual*/ mafCore::mafContainerInterface *calculateBoundary(double bounds[6], mafPoseMatrix *matrix = NULL);

    /// Return bounds of the bounding box.
    /*virtual*/ void bounds(double bounds[6]);

    /// Return the m_BoundaryItem.
    mafString boundaryItem();

private:
    mafString m_BoundaryItem; ///< Test Var.
};

testDataBoundaryAlgorithmCustom::testDataBoundaryAlgorithmCustom(const mafString code_location) : mafDataBoundaryAlgorithm(code_location), m_BoundaryItem("") {
}

mafString testDataBoundaryAlgorithmCustom::boundaryItem() {
    return m_BoundaryItem;
}

mafContainerInterface *testDataBoundaryAlgorithmCustom::calculateBoundary(mafContainerInterface *data, mafPoseMatrix *matrix) {
    Q_UNUSED(data);
    Q_UNUSED(matrix);

    m_BoundaryItem = "BoundaryCalculated";
    return NULL;
}

mafCore::mafContainerInterface *testDataBoundaryAlgorithmCustom::calculateBoundary(double bounds[6], mafPoseMatrix *matrix){
    Q_UNUSED(bounds);
    Q_UNUSED(matrix);

    m_BoundaryItem = "BoundaryCalculated";
    return NULL;
}

void testDataBoundaryAlgorithmCustom::bounds(double bounds[6]) {
}
//------------------------------------------------------------------------------------------

/**
 Class name: mafDataBoundaryAlgorithmTest
 This class implements the test suite for mafInterpolator.
 */
class mafDataBoundaryAlgorithmTest: public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        m_BoundaryAlgorithm = mafNEW(testDataBoundaryAlgorithmCustom);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_BoundaryAlgorithm);
    }

    /// testDataBoundaryAlgorithmCustom allocation test case.
    void mafBoundaryAlgorithmAllocationTest();
    /// Test the boundary algorithm strategy.
    void mafBoundaryAlgorithmStrategyTest();

private:
    testDataBoundaryAlgorithmCustom *m_BoundaryAlgorithm; ///< Test var.
};

void mafDataBoundaryAlgorithmTest::mafBoundaryAlgorithmAllocationTest() {
    QVERIFY(m_BoundaryAlgorithm != NULL);
}

void mafDataBoundaryAlgorithmTest::mafBoundaryAlgorithmStrategyTest() {
    mafString res("BoundaryCalculated");
    m_BoundaryAlgorithm->calculateBoundary(NULL);
    QCOMPARE(m_BoundaryAlgorithm->boundaryItem(), res);
}


MAF_REGISTER_TEST(mafDataBoundaryAlgorithmTest);
#include "mafDataBoundaryAlgorithmTest.moc"

