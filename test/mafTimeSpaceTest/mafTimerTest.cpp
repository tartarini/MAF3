/*
 *  mafTimerTest.cpp
 *  mafTimeSpaceTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <QEventLoop>

#include <mafCoreSingletons.h>
#include <mafCoreRegistration.h>
#include <mafEventBusManager.h>
#include <mafTimer.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafTimeSpace;

bool startTimerOnEventLoop(mafTimer *timer, int timeout) {
    QEventLoop loop; // Event loop needed for the timer.
    QTimer timerLoop; // Timer that will terminate the event loop.
    timerLoop.setSingleShot(timer->isSingleShot());
    QObject::connect(&timerLoop, SIGNAL(timeout()), &loop, SLOT(quit()));
    if (timeout > 0) {
        timerLoop.start(timeout);
        timer->start(timeout);
    }
    loop.exec();
    return true;
}

//-------------------------------------------------------------------------
/**
 Class name: testObjectCustom
 Custom object needed for testing.
 */

class testObjectCustom : public mafObject {
    Q_OBJECT

public:
    /// constructor.
    testObjectCustom(const QString code_location = "");

    /// Return tha var's value.
    int var() {return m_Var;}

    /// set the type of timer test is executed.
    void setOneShotTest(bool one_shot = true);

    /// Reset the internal var value.
    void resetVar() {m_Var = 0;}

public slots:
    /// Test slot that will receive the timeoutReached event.
    void updateObject();

private:
    int m_Var; ///< Test var.
    bool m_OneShot; ///< Test var.
};

testObjectCustom::testObjectCustom(const QString code_location) : mafObject(code_location), m_Var(0), m_OneShot(true) {
}

void testObjectCustom::updateObject() {
    ++m_Var;
    qDebug() << m_Var;
    if(m_OneShot) {
        QVERIFY(m_Var == 1);
    }

    qDebug("timeout reached!!");
}
//-------------------------------------------------------------------------


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

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_TimerTest = mafNEW(mafTimeSpace::mafTimer);
        m_ObjectTest = mafNEW(testObjectCustom);
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_TimerTest);
        mafDEL(m_ObjectTest);
        mafEventBusManager::instance()->shutdown();
    }

    /// mafTimer allocation test case.
    void mafTimerAllocationTest();
    /// one-hsot timing test case.
    void startOneShotTest();

private:
    mafTimer *m_TimerTest; ///< Test var
    testObjectCustom *m_ObjectTest; ///< Test object that observe the timer events.
};

void mafTimerTest::mafTimerAllocationTest() {
    QVERIFY(m_TimerTest != NULL);
}

void mafTimerTest::startOneShotTest() {
    QString obj_id = mafIdProvider::instance()->idName(m_TimerTest->objectId());
    mafRegisterLocalCallback(obj_id, m_ObjectTest, "updateObject()");

    m_TimerTest->setSingleShot(true);

    // Use accessory function for defining event loop (not needed in applications) and start the timer.
    startTimerOnEventLoop(m_TimerTest, 500);

    qDebug() << "var value = " << m_ObjectTest->var();
}

MAF_REGISTER_TEST(mafTimerTest);
#include "mafTimerTest.moc"
