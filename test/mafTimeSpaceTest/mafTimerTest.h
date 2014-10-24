/*
 *  mafTimerTest.h
 *  mafTimeSpaceTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef mafTimerTest_H
#define mafTimerTest_H
 
#include <mafTestSuite.h>
#include <mafTimer.h>

/// forward declaration
class testObjectCustom;
 
/**
 Class name: mafTimerTest
 This class implements the test suite for mafTimer.
 */


//! <title>
//mafTimer
//! </title>
//! <description>
//This class provides the wrapper for the QTimer necessary to send events through the Event Bus.
//By default the constructed timer is single shot. To enable cyclic timing use setSingleShot method and
//pass to it a 'false' boolean value. All the timing value (intervals) are expressed in milliseconds.
//! </description>

class mafTimerTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// mafTimer allocation test case.
    void mafTimerAllocationTest();
    /// one-hsot timing test case.
    void startOneShotTest();

private:
    mafTimeSpace::mafTimer *m_TimerTest; ///< Test var
    testObjectCustom *m_ObjectTest; ///< Test object that observe the timer events.
};

#endif //mafTimerTest_H