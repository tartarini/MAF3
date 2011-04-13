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

#include "mafTimeManager.h"
#include "mafEventBusManager.h"
#include <mafCoreSingletons.h>
#include "mafTimer.h"
#include <mafObject.h>

#define interval 500

#ifdef WIN32
    #ifdef QT_DEBUG
        #define TIME_SPACE_LIBRARY_NAME "mafTimeSpace.dll"
    #else
        #define TIME_SPACE_LIBRARY_NAME "mafTimeSpace.dll"
    #endif
#else
    #ifdef __APPLE__
        #ifdef QT_DEBUG
            #define TIME_SPACE_LIBRARY_NAME "mafTimeSpace.dylib"
        #else
            #define TIME_SPACE_LIBRARY_NAME "mafTimeSpace.dylib"
        #endif
    #else
        #ifdef QT_DEBUG
            #define TIME_SPACE_LIBRARY_NAME "mafTimeSpace.so"
        #else
            #define TIME_SPACE_LIBRARY_NAME "mafTimeSpace.so"
        #endif
    #endif
#endif

using namespace mafCore;
using namespace mafEventBus;
using namespace mafTimeSpace;

//-------------------------------------------------------------------------
/**
 Class name: testObjectCustomListener
 Custom object needed for testing.
 */
class testObjectCustomListener : public mafObject {
    Q_OBJECT

public:
    /// constructor.
    testObjectCustomListener(const QString code_location = "");

    /// Return tha var's value.
    int var() {return m_Var;}

    /// set the type of timer test is executed.
    void setOneShotTest(bool one_shot = true);

    /// reset the value of the variable.
    void resetVar() {m_Var = 0;}

public slots:
    /// Test slot that will receive the timeoutReached event.
    void updateObject();

private:
    int m_Var; ///< Test var.
    bool m_OneShot; ///< Test var.
};

testObjectCustomListener::testObjectCustomListener(const QString code_location) : mafObject(code_location), m_Var(0), m_OneShot(true) {
}

void testObjectCustomListener::updateObject() {
    ++m_Var;
    qDebug() << m_Var;
    if(m_OneShot) {
        QVERIFY(m_Var == 1);
    }

    qDebug("timeout from manager reached!!");
}
//-------------------------------------------------------------------------


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

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_TimerId = -1;
        m_EventBus = mafEventBusManager::instance();
        m_ObjectTest = mafNEW(testObjectCustomListener);
        m_TimeManager = mafTimeManager::instance();
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_ObjectTest);
        m_TimeManager->shutdown();
        m_EventBus->shutdown();
    }

    /// Test library loading
    void mafTimeSpaceLibraryLoading();
    /// mafTimeManager allocation test case.
    void mafTimeManagerAllocationTest();
    /// timer creation test case.
    void createTimerTest();
    /// timer start test case.
    void startTimerTest();
    /// timer start on different thread test case.
    void startTimerOnThreadTest();

private:
    mafEventBusManager *m_EventBus; ///< Event Bus manager instance.
    mafTimeManager *m_TimeManager; ///< Test var
    testObjectCustomListener *m_ObjectTest; ///< Test object that observe the timer events.
    mafId m_TimerId; ///< Test var that store the created timer's id.
};

void mafTimeManagerTest::mafTimeSpaceLibraryLoading() {
    bool module_initialized(false);
    module_initialized = mafInitializeModule(TIME_SPACE_LIBRARY_NAME);
    QVERIFY(module_initialized);
}

void mafTimeManagerTest::mafTimeManagerAllocationTest() {
    QVERIFY(m_TimeManager != NULL);
}

void mafTimeManagerTest::createTimerTest() {
//    m_TimerId = m_TimeManager->createTimer(interval, m_ObjectTest, "updateObject()", true);
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(int, interval));
    argList.append(mafEventArgument(bool, true));

    QGenericReturnArgument value = mafEventReturnArgument(mafCore::mafId, m_TimerId);
    m_EventBus->notifyEvent("maf.local.time.create", mafEventTypeLocal, &argList, &value);
    QVERIFY(m_TimerId != -1);

    // Register the callback with the timer ID. Later on will be used in the test suite.
    QString timer_id = mafIdProvider::instance()->idName(m_TimerId);
    mafRegisterLocalCallback(timer_id, m_ObjectTest, "updateObject()");

    // The created timer is not active; call the mafTimeManager's startTimer method
    // with timer's ID to start the corresponding timer
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafId, m_TimerId));

    bool res(true);
    QGenericReturnArgument return_value = mafEventReturnArgument(bool, res);
    m_EventBus->notifyEvent("maf.local.time.isRunning", mafEventTypeLocal, &argList, &return_value);

    QVERIFY(res == false);
}

void mafTimeManagerTest::startTimerTest() {
    QEventLoop loop; // Event loop needed for the timer.
    QTimer timerLoop; // Timer that will terminate the event loop.
    timerLoop.setSingleShot(true);
    QObject::connect(&timerLoop, SIGNAL(timeout()), &loop, SLOT(quit()));

    // Start the timer in the main thread
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafId, m_TimerId));
    argList.append(mafEventArgument(bool, false));
    m_EventBus->notifyEvent("maf.local.time.start", mafEventTypeLocal, &argList);

//    m_TimeManager->startTimer(m_TimerId, false);
    timerLoop.start(interval);
    loop.exec();

    qDebug() << "var value = " << m_ObjectTest->var();
}
void mafTimeManagerTest::startTimerOnThreadTest() {
    m_ObjectTest->resetVar();

    // Start the timer in a separate thread.
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafId, m_TimerId));
    argList.append(mafEventArgument(bool, true));
    m_EventBus->notifyEvent("maf.local.time.start", mafEventTypeLocal, &argList);

//    m_TimeManager->startTimer(m_TimerId, true);

    QTest::qWait(2000);

    qDebug() << "var value = " << m_ObjectTest->var();
}

MAF_REGISTER_TEST(mafTimeManagerTest);
#include "mafTimeManagerTest.moc"
