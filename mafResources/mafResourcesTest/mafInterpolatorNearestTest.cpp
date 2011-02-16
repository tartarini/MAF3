/*
 *  mafInterpolatorNearestTest.cpp
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
#include <mafInterpolatorNearest.h>

using namespace mafCore;
using namespace mafResources;

/**
 Class name: mafInterpolatorNearestTest
 This class implements the test suite for mafInterpolatorNearest.
 */

//! <title>
//mafInterpolatorNearest
//! </title>
//! <description>
//mafInterpolatorNearest provides the mechanism to return the element nearest the the given timestamp.
//First element or last one are returned for timestamps outside the definition range of the collection.
//! </description>

class mafInterpolatorNearestTest: public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();

        //! <snippet>
        m_Interpolator = mafNEW(mafResources::mafInterpolatorNearest);
        //! </snippet>

        double ts[10] = {0.5, 1.5, 10.5, 11.0, 12.8, 23.9, 52.1, 53.0, 53.1, 53.2};
        for(int i = 0; i < 10; ++i) {
            mafDataSet *item = mafNEW(mafResources::mafDataSet);
            m_ItemMap.insert(ts[i], item);
            delete item;
        }
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        m_ItemMap.clear();
        mafDEL(m_Interpolator);
        mafEventBus::mafEventBusManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafInterpolatorNearest allocation test case.
    void mafInterpolatorNearestAllocationTest();
    /// Test the interpolation strategy.
    void mafInterpolationNearestStrategyTest();
    /// Test the interpolation empty collection.
    void mafInterpolationNearestEmptyCollTest();

private:
    mafInterpolator *m_Interpolator; ///< Test var.
    mafDataSetMap m_ItemMap; ///< Test data collection.
};

void mafInterpolatorNearestTest::mafInterpolatorNearestAllocationTest() {
    QVERIFY(m_Interpolator != NULL);
}

void mafInterpolatorNearestTest::mafInterpolationNearestStrategyTest() {
    mafDataSet *item;

    // item returned is at a time before the begin of the map; the nearest item is the first one.
    item = m_Interpolator->itemAt(&m_ItemMap, 0.0);
    QVERIFY(item != NULL);

    // Try to ask an element at a time over the timestemp max; the last element hsould be returned.
    //! <snippet>
    item = m_Interpolator->itemAt(&m_ItemMap, 100.5);
    //! </snippet>
    QVERIFY(item != NULL);

    QBENCHMARK {
        item = m_Interpolator->itemAt(&m_ItemMap, 48.0);
    }

    QVERIFY(item != NULL);
}

void mafInterpolatorNearestTest::mafInterpolationNearestEmptyCollTest() {
    m_ItemMap.clear();

    mafDataSet *item;
    // Collection empty; NULL should be returned.
    item = m_Interpolator->itemAt(&m_ItemMap, 48.0);
    QVERIFY(item == NULL);

    item = mafNEW(mafResources::mafDataSet);
    m_ItemMap.insert(0.0, item);
    delete item;

    item = m_Interpolator->itemAt(&m_ItemMap, 0.0);
    QVERIFY(item != NULL);
}

MAF_REGISTER_TEST(mafInterpolatorNearestTest);
#include "mafInterpolatorNearestTest.moc"
