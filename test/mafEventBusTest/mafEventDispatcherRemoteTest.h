/*
 *  mafEventDispatcherRemoteTest.h
 *  mafEventBusTest
 *
 *  Created by Daniele Giunchi on 20/04/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafEventDispatcherRemote.h>

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
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafEventDispatcherRemote allocation test case.
    void mafEventDispatcherRemoteAllocationTest();

    /// mafEventDispatcherRemote accessors test case: test client and server pointers.
    void mafEventDispatcherRemoteAccessorsTest();

private:
    mafEventBus::mafEventDispatcherRemote *m_EventDispatcherRemote; ///< Test var.
};
