/*
 *  mafEventDispatcherLocalTest.h
 *  mafEventBusTest
 *
 *  Created by Daniele Giunchi on 20/04/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafEventDispatcherLocal.h>
#include <mafEvent.h>

class testObjectCustomForDispatcherLocal;

/**
 Class name: mafEventDispatcherLocalTest
 This class implements the test suite for mafEventDispatcherLocal.
 */

//! <title>
//mafEventDispatcherLocal
//! </title>
//! <description>
//mafEventDispatcherLocal allows dispatching events coming from local
//application to attached observers.
//! </description>

class mafEventDispatcherLocalTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();
    
    /// mafEventDispatcherLocal allocation test case.
    void mafEventDispatcherLocalAllocationTest();

    /// notify event test which cover all the possibilities in terms of arguments
    void notifyEventWithoutReturnValueTest();

    /// notify event test which cover all the possibilities in terms of arguments with returned value
    void notifyEventWitReturnValueTest();

private:
    testObjectCustomForDispatcherLocal *m_ObjTest; ///< Test Object var
    mafEventBus::mafEventDispatcherLocal *m_EventDispatcherLocal; ///< Test var.
};
