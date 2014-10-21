/*
 *  mafTimeManagerTest.h
 *  mafTimeSpaceTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef mafTimeManagerTest_H
#define mafTimeManagerTest_H
 
#include <mafTestSuite.h>

#include <mafEventBusManager.h>
#include <mafTimeManager.h>

class testObjectCustomListener;

 /**
 Class name: mafTimeManagerTest
 This class implements the test suite for mafTimeManager.
 */

 //! <title>
//mafTimeManager
//! </title>
//! <description>
//This singletone provides the facade class for time managing.
//! </description>

class mafTimeManagerTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// Test library loading
    void mafTimeSpaceLibraryLoading();
    /// mafTimeManager allocation test case.
    void mafTimeManagerAllocationTest();
    /// timer creation test case.
    void createTimerTest();
    /// timer start test case.
    void startTimerTest();
    /// timer start on different thread test case.
    //void startTimerOnThreadTest();

private:
    mafEventBus::mafEventBusManager *m_EventBus; ///< Event Bus manager instance.
    mafTimeSpace::mafTimeManager *m_TimeManager; ///< Test var
    testObjectCustomListener *m_ObjectTest; ///< Test object that observe the timer events.
    mafId m_TimerId; ///< Test var that store the created timer's id.
};

#endif //mafTimeManagerTest_H