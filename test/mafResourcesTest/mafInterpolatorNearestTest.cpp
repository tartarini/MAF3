/*
 *  mafInterpolatorNearestTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 22/09/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

using namespace mafCore;
using namespace mafResources;

void mafInterpolatorNearestTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();

    //! <snippet>
    m_Interpolator = mafNEW(mafResources::mafInterpolatorNearest);
    //! </snippet>

    double ts[10] = {0.5, 1.5, 10.5, 11.0, 12.8, 23.9, 52.1, 53.0, 53.1, 53.2};
    for(int i = 0; i < 10; ++i) {
        mafDataSet *item = mafNEW(mafResources::mafDataSet);
        m_ItemMap.insert(ts[i], item);
        mafDEL(item);
    }
}

void mafInterpolatorNearestTest::cleanupTestCase() {
    m_ItemMap.clear();
    mafDEL(m_Interpolator);
    mafEventBus::mafEventBusManager::instance()->shutdown();
    mafMessageHandler::instance()->shutdown();
}

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
    mafDEL(item);

    item = m_Interpolator->itemAt(&m_ItemMap, 0.0);
    QVERIFY(item != NULL);
}

#include "mafInterpolatorNearestTest.moc"
