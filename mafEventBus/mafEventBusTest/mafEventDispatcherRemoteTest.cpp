/*
 *  mafEventDispatcherRemoteTest.cpp
 *  mafEventBusTest
 *
 *  Created by Daniele Giunchi on 20/04/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafEventDispatcherRemote.h>

using namespace mafCore;
using namespace mafEventBus;

/**
 Class name: mafEventDispatcherRemoteTest
 This class implements the test suite for mafEventDispatcherRemote.
 */

//! <title>
//mafEventDispatcherRemote
//! </title>
//! <description>
//mafEventDispatcherRemote allows dispatching events coming from local
// application to attached observers.
//! </description>

class mafEventDispatcherRemoteTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_EventDispatcherLocal = new mafEventDispatcherRemote;
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        delete m_EventDispatcherLocal;
    }

    /// mafEventDispatcherRemote allocation test case.
    void mafEventDispatcherRemoteAllocationTest();

private:
    mafEventDispatcherRemote *m_EventDispatcherLocal; ///< Test var.
};

void mafEventDispatcherRemoteTest::mafEventDispatcherRemoteAllocationTest() {
    QVERIFY(m_EventDispatcherLocal != NULL);
}

MAF_REGISTER_TEST(mafEventDispatcherRemoteTest);
#include "mafEventDispatcherRemoteTest.moc"



