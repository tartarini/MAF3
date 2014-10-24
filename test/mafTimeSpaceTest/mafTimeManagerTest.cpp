/*
 *  mafTimeManagerTest.cpp
 *  mafTimeSpaceTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 22/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTimeSpaceTestList.h"
#include <QEventLoop>

#include <mafCoreSingletons.h>
#include "mafTimer.h"
#include <mafObject.h>

#define interval 500

#if defined(_WIN32) || defined(WIN32)
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

public Q_SLOTS:
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

void mafTimeManagerTest::initTestCase() {
    m_TimerId = -1;
    m_EventBus = mafEventBusManager::instance();
    m_ObjectTest = mafNEW(testObjectCustomListener);
    m_TimeManager = mafTimeManager::instance();
}
    
void mafTimeManagerTest::cleanupTestCase() {
    mafDEL(m_ObjectTest);
    m_TimeManager->shutdown();
    m_EventBus->shutdown();
}

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
/*void mafTimeManagerTest::startTimerOnThreadTest() {
    m_ObjectTest->resetVar();

    // Start the timer in a separate thread.
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafId, m_TimerId));
    argList.append(mafEventArgument(bool, true));
    m_EventBus->notifyEvent("maf.local.time.start", mafEventTypeLocal, &argList);

//    m_TimeManager->startTimer(m_TimerId, true);

    QTest::qWait(2000);

    qDebug() << "var value = " << m_ObjectTest->var();
}*/


#include "mafTimeManagerTest.moc"
