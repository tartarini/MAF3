/*
 *  mafEventDispatcherTest.h
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTestSuite.h"
#include <mafEventBusManager.h>

class testObjectCustomForDispatcher;

/**
 Class name: mafEventDispatcherTest
 This class implements the test suite for mafEventDispatcher.
 */

//! <title>
//mafEventDispatcher
//! </title>
//! <description>
//mafEventDispatcher allows dispatching events coming from local application
//to attached observers.
//! </description>

class mafEventDispatcherTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafEventDispatcher allocation test case.
    void mafEventDispatcherAllocationTest();
    /// Test the add and remove observer.
    void mafEventDispatcherAddAndRemoveObserverAndNotifyEventTest();
    /// Test the add and remove signal.
    void mafEventDispatcherRegisterAndRemoveSignalAndNotifyEventTest();
    /// Test if the signal is present after registration
    void isSignalPresentTest();
    /// Remove observer given a pointer.
    void removeObserverTest();
    /// Remove item from the dispatcher.
    void removeItemTest();
    /// Remove signal given a pointer.
    void removeSignalTest();
    /// test if the local signal is present
    void isLocalSignalPresentTest();
    /// reverse order registration test
    void reverseOrderRegistrationTest();

private:
    mafEventBus::mafEventDispatcher *m_EventDispatcher; ///< Test var.
    testObjectCustomForDispatcher *m_ObjTestObserver; ///< Test Object var
};
