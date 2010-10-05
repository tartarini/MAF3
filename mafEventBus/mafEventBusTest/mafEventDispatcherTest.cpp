/*
 *  mafEventDispatcherTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTestSuite.h"
#include <mafEventBusManager.h>

using namespace mafEventBus;

//-------------------------------------------------------------------------
/**
 Class name: mafObjectCustom
 Custom object needed for testing.
 */
class testObjectCustomForDispatcher : public QObject {
    Q_OBJECT

public:
    /// constructor.
    testObjectCustomForDispatcher();

    /// Return the var's value.
    int var() {return m_Var;}

public slots:
    /// Test slot that will increment the value of m_Var when an UPDATE_OBJECT event is raised.
    void updateObject();
    void setObjectValue(int v);

signals:
    void valueModified(int v);
    void objectModified();

private:
    int m_Var; ///< Test var.
};

testObjectCustomForDispatcher::testObjectCustomForDispatcher() : QObject(), m_Var(0) {
}

void testObjectCustomForDispatcher::updateObject() {
    m_Var++;
}

void testObjectCustomForDispatcher::setObjectValue(int v) {
    m_Var = v;
}

//-------------------------------------------------------------------------


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

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_ObjTestObserver = new testObjectCustomForDispatcher;
        m_EventDispatcher = new mafEventBus::mafEventDispatcher;
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        delete m_ObjTestObserver;
        delete m_EventDispatcher;
    }

    /// mafEventDispatcher allocation test case.
    void mafEventDispatcherAllocationTest();
    /// Test the add and remove observer.
    void mafEventDispatcherAddAndRemoveObserverAndNotifyEventTest();

    /// Test the add and remove signal.
    void mafEventDispatcherRegisterAndRemoveSignalAndNotifyEventTest();

    /// Test if the signal is present after registration
    void isSignalPresentTest();

private:
    mafEventDispatcher *m_EventDispatcher; ///< Test var.
    testObjectCustomForDispatcher *m_ObjTestObserver; ///< Test Object var
};

void mafEventDispatcherTest::mafEventDispatcherAllocationTest() {
    QVERIFY(m_EventDispatcher != NULL);
}

void mafEventDispatcherTest::mafEventDispatcherAddAndRemoveObserverAndNotifyEventTest() {
    // Create new Event ID used for callback and event notification.
    mafString updateID = "GLOBAL_UPDATE_EVENT";

    mafEvent *properties = new mafEventBus::mafEvent;
    (*properties)[TOPIC] =  updateID;
    (*properties)[TYPE] = mafEventTypeLocal;
    (*properties)[SIGTYPE] = mafSignatureTypeSignal;
    QVariant var;
    var.setValue((QObject*)m_EventDispatcher);
    (*properties)[OBJECT] = var;
    (*properties)[SIGNATURE] = "notifyDefaultEvent()";
    QVERIFY(m_EventDispatcher->registerSignal(*properties));

    mafEvent *propCallback = new mafEventBus::mafEvent;
    (*propCallback)[TOPIC] =  updateID;
    (*propCallback)[TYPE] = mafEventTypeLocal;
    (*propCallback)[SIGTYPE] = mafSignatureTypeCallback;
    QVariant varobserver;
    varobserver.setValue((QObject*)m_ObjTestObserver);
    (*propCallback)[OBJECT] = varobserver;
    (*propCallback)[SIGNATURE] = "updateObject()";
    QVERIFY(m_EventDispatcher->addObserver(*propCallback));
}

void mafEventDispatcherTest::mafEventDispatcherRegisterAndRemoveSignalAndNotifyEventTest() {
    mafString updateID = "GLOBAL_UPDATE_EVENT";

    mafEvent *properties = new mafEventBus::mafEvent;
    (*properties)[TOPIC] =  updateID;
    (*properties)[TYPE] = mafEventTypeLocal;
    (*properties)[SIGTYPE] = mafSignatureTypeSignal;
    QVariant var;
    var.setValue((QObject*)m_EventDispatcher);
    (*properties)[OBJECT] = var;
    (*properties)[SIGNATURE] = "notifyDefaultEvent()";
    QVERIFY(m_EventDispatcher->removeSignal(*properties));

    QVERIFY(m_EventDispatcher->registerSignal(*properties));

    // Register the callback to update the object custom:
    mafEvent *propCallback = new mafEventBus::mafEvent;
    (*propCallback)[TOPIC] =  updateID;
    (*propCallback)[TYPE] = mafEventTypeLocal;
    (*propCallback)[SIGTYPE] = mafSignatureTypeCallback;
    QVariant varobserver;
    varobserver.setValue((QObject*)m_ObjTestObserver);
    (*propCallback)[OBJECT] = varobserver;
    (*propCallback)[SIGNATURE] = "updateObject()";
    QVERIFY(m_EventDispatcher->addObserver(*propCallback));
}

void mafEventDispatcherTest::isSignalPresentTest() {
    mafString updateID = "GLOBAL_UPDATE_EVENT";
    // Register the callback to update the object custom:
    QVERIFY(m_EventDispatcher->isSignalPresent(updateID));
}


MAF_REGISTER_TEST(mafEventDispatcherTest);
#include "mafEventDispatcherTest.moc"
