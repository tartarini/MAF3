/*
 *  mafInterpolatorBeforeTest.cpp
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
#include <mafInterpolatorBefore.h>

using namespace mafCore;
using namespace mafResources;

/**
 Class name: mafInterpolatorBeforeTest
 This class implements the test suite for mafInterpolatorBefore.
 */

 //! <title>
//mafInterpolatorBefore
//! </title>
//! <description>
//mafInterpolatorBefore provides the mechanism to return the element suddenly before the
//given timestamp. If no elements are defined befor the given timestamp, NULL is returned.
//! </description>
class mafInterpolatorBeforeTest: public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();

        //! <snippet>
        m_Interpolator = mafNEW(mafResources::mafInterpolatorBefore);
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

    /// mafInterpolatorBefore allocation test case.
    void mafInterpolatorBeforeAllocationTest();
    /// Test the interpolation strategy.
    void mafInterpolationBeforeStrategyTest();

private:
    mafInterpolator *m_Interpolator; ///< Test var.
    mafDataSetMap m_ItemMap; ///< Test data collection.
};

void mafInterpolatorBeforeTest::mafInterpolatorBeforeAllocationTest() {
    QVERIFY(m_Interpolator != NULL);
}

void mafInterpolatorBeforeTest::mafInterpolationBeforeStrategyTest() {
    mafDataSet *item;

    // item returned is at a time before the begin of the map; NULL should be returned.
    item = m_Interpolator->itemAt(&m_ItemMap, 0.0);
    QVERIFY(item == NULL);

    // Retrieve the first elemnt.
    //! <snippet>
    item = m_Interpolator->itemAt(&m_ItemMap, 0.5);
    //! </snippet>
    QVERIFY(item != NULL);

    // Try to ask an element at a time over the timestemp max; the last element hsould be returned.
    item = m_Interpolator->itemAt(&m_ItemMap, 100.5);
    QVERIFY(item != NULL);

    QBENCHMARK {
        item = m_Interpolator->itemAt(&m_ItemMap, 48.0);
    }

    QVERIFY(item != NULL);
}

MAF_REGISTER_TEST(mafInterpolatorBeforeTest);
#include "mafInterpolatorBeforeTest.moc"
