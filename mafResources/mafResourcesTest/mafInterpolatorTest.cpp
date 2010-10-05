/*
 *  mafInterpolatorTest.cpp
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
#include <mafInterpolator.h>

using namespace mafCore;
using namespace mafResources;

//------------------------------------------------------------------------------------------
/**
 Class name: testInterpolatorCustom
 This class implements the interpolator to be tested.
 */
class  testInterpolatorCustom : public  mafInterpolator {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafInterpolator);

public:
    /// Object constructor.
    testInterpolatorCustom(const mafString code_location = "");

    /// Search the item at the given timestamp 't' with the defined interpolation strategy.
    /*virtual*/ mafDataSet *itemAt(mafDataSetMap *collection, double t);

    /// Access the result of the interpolation mechanism.
    mafString interpolatedItem() {return m_InterpolatedItem;}

private:
    mafString m_InterpolatedItem; ///< Test Var.
};

testInterpolatorCustom::testInterpolatorCustom(const mafString code_location) : mafInterpolator(code_location), m_InterpolatedItem("") {
}

mafDataSet *testInterpolatorCustom::itemAt(mafDataSetMap *collection, double t) {
    Q_UNUSED(collection);
    Q_UNUSED(t);
    m_InterpolatedItem = "Interpolated";
    return NULL;
}
//------------------------------------------------------------------------------------------


/**
 Class name: mafInterpolatorTest
 This class implements the test suite for mafInterpolator.
 */
class mafInterpolatorTest: public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_Interpolator = mafNEW(testInterpolatorCustom);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Interpolator);
    }

    /// mafInterpolatorBefore allocation test case.
    void mafInterpolatorAllocationTest();
    /// Test the interpolation strategy.
    void mafInterpolationStrategyTest();

private:
    testInterpolatorCustom *m_Interpolator; ///< Test var.
    mafDataSetMap m_ItemMap; ///< Test data collection.
};

void mafInterpolatorTest::mafInterpolatorAllocationTest() {
    QVERIFY(m_Interpolator != NULL);
}

void mafInterpolatorTest::mafInterpolationStrategyTest() {
    mafString res("Interpolated");
    mafDataSet *data = m_Interpolator->itemAt(&m_ItemMap, 0.0);
    QVERIFY(data == NULL);
    QCOMPARE(m_Interpolator->interpolatedItem(), res);
}

MAF_REGISTER_TEST(mafInterpolatorTest);
#include "mafInterpolatorTest.moc"
