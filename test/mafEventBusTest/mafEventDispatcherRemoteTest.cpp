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
#include <mafEventDispatcherRemote.h>

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

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        m_EventDispatcherRemote = new mafEventDispatcherRemote();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        delete m_EventDispatcherRemote;
    }

    /// mafEventDispatcherRemote allocation test case.
    void mafEventDispatcherRemoteAllocationTest();

    /// mafEventDispatcherRemote accessors test case: test client and server pointers.
    void mafEventDispatcherRemoteAccessorsTest();

private:
    mafEventDispatcherRemote *m_EventDispatcherRemote; ///< Test var.
};

void mafEventDispatcherRemoteTest::mafEventDispatcherRemoteAllocationTest() {
    QVERIFY(m_EventDispatcherRemote != NULL);
}

void mafEventDispatcherRemoteTest::mafEventDispatcherRemoteAccessorsTest() {
    QVERIFY(m_EventDispatcherRemote->networkConnectorClient() == NULL);
    QVERIFY(m_EventDispatcherRemote->networkConnectorServer() == NULL);
}

MAF_REGISTER_TEST(mafEventDispatcherRemoteTest);
#include "mafEventDispatcherRemoteTest.moc"



