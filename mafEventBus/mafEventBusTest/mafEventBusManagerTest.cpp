/*
 *  mafEventBusManagerTest.cpp
 *  mafEventBusManagerTest
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafEventBusManager.h>
#include <mafEventDefinitions.h>

using namespace mafEventBus;

//-------------------------------------------------------------------------
/**
 Class name: testObjectCustom
 Custom object needed for testing.
 */
class testObjectCustom : public QObject {
    Q_OBJECT

public:
    /// constructor.
    testObjectCustom();

    /// Return tha var's value.
    int var() {return m_Var;}

public slots:
    /// Test slot that will increment the value of m_Var when an UPDATE_OBJECT event is raised.
    void updateObject();
    void setObjectValue(int v);
    int returnObjectValue();

signals:
    void valueModified(int v);
    void objectModified();
    int returnObjectValueSignal();

private:
    int m_Var; ///< Test var.
};

testObjectCustom::testObjectCustom() : m_Var(0) {
}

void testObjectCustom::updateObject() {
    m_Var++;
}

void testObjectCustom::setObjectValue(int v) {
    m_Var = v;
}

int testObjectCustom::returnObjectValue() {
    int value = 5;
    return value;
}

//-------------------------------------------------------------------------

/**
 Class name: mafEventBusManagerTest
 This class implements the test suite for mafEventBusManager.
 */

//! <title>
//mafEventBusManager
//! </title>
//! <description>
//mafEventBusManager provides the access point of the Communication Bus for MAF3 framework and
//allows dispatching events coming from local application to attached observers.
//It provides APIs to add a new event property (observer or event) to the event bus hash or
//to remove the event property from the event bus hash.
//It also provides APIs to add or remove observer to the events and to register custom signals
//use by objects to raise them events.
//
//Sender has to:
//- create new Event ID used for callback and event notification (using mafIdProvider)
//- Register a signal (with argument if necessary)
//
//Observer has to:
//- Register the callback that will be called when event will be notified (with argument if necessary)
// with the same Event ID used by the sender
//
//The method "notifyEventLocal(const mafCore::mafId id, mafEventArgumentsList *argList)" accept a mafId and a list
//of QGenericArgument: to set QGenericArgument use the mafEventArgument() macros.
//mafEventArgument() takes a type name and a const reference of that type.
//! </description>

class mafEventBusManagerTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_EventBus = mafEventBusManager::instance();
        m_ObjTestObserver = new testObjectCustom();
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        delete m_ObjTestObserver;
        m_EventBus->shutdown();
    }

    /// Check the existence of the mafEventBusManager singletone creation.
    void mafEventBusManagerConstructorTest();

    /// Check the event observing registration and notification.
    void eventBusRegistrationNotificationTest();

    /// Check the event observing registration and notification passing one argument.
    void eventBusWithArgumentTest();

    /// Check the event observing registration and notification returning one argument.
    void eventBusWithReturnArgumentTest();

    /// Event notification benchmarks.
    void eventNotificationBenchmarkTest();

private:
    testObjectCustom *m_ObjTestObserver; ///< Test variable.
    mafEventBusManager *m_EventBus; ///< EventBus test variable instance.
};

void mafEventBusManagerTest::mafEventBusManagerConstructorTest() {
    QVERIFY(m_EventBus != NULL);
}

void mafEventBusManagerTest::eventBusRegistrationNotificationTest() {
    int status = m_ObjTestObserver->var();
    QVERIFY(status == 0);

    // Create new Event ID used for callback and event notification.
    mafString updateID = "GLOBAL_UPDATE_EVENT";
    mafRegisterLocalCallback(updateID, m_ObjTestObserver, "updateObject()");

    // Notify the update event... (event logging is disabled)
    m_EventBus->notifyEvent("GLOBAL_UPDATE_EVENT");

    status = m_ObjTestObserver->var();
    QVERIFY(status == 1);

    // Notify again but with event logging enabled...
    m_EventBus->enableEventLogging();
    m_EventBus->notifyEvent(updateID);

    // ... and now filter events only with ID == "CUSTOM_SIGNAL"
    //mafId customID = idProvider->createNewId("CUSTOM_SIGNAL");
    mafString customID = "CUSTOM_SIGNAL";

    // ...and enable event logging for that id.
    m_EventBus->logEventTopic(customID);

    // Previous ID notification should be skipped by logger (so only one log in the console)
    m_EventBus->notifyEvent(updateID);
}

 void mafEventBusManagerTest::eventBusWithArgumentTest() {
    testObjectCustom *ObjTestSender = new testObjectCustom();
    ObjTestSender->setObjectValue(52);

    // Create new Event ID used for callback and event notification.
    mafString setValueID = "SETVALUE_SIGNAL";
    mafRegisterLocalSignal(setValueID, ObjTestSender, "valueModified(int)");

    // Register the callback to update the object custom:
    mafRegisterLocalCallback(setValueID, m_ObjTestObserver, "setObjectValue(int)");

    //! <snippet>
    mafEventArgumentsList list;
    list.append(mafEventArgument(int, ObjTestSender->var()));

    m_EventBus->notifyEvent(setValueID, mafEventTypeLocal, &list);
    //! </snippet>
    int status = m_ObjTestObserver->var();
    QVERIFY(status == ObjTestSender->var());
    delete ObjTestSender;
}

void mafEventBusManagerTest::eventBusWithReturnArgumentTest() {
    testObjectCustom *ObjTestSender = new testObjectCustom();
    ObjTestSender->setObjectValue(52);

    // Create new Event ID used for callback and event notification.
    mafString returnValueID = "RETURNVALUE_SIGNAL";
    mafRegisterLocalSignal(returnValueID, ObjTestSender, "returnObjectValue()");


    //Notify event with return argument
    int returnValue = 0;
    m_EventBus->notifyEvent(returnValueID, mafEventTypeLocal, NULL, &mafEventReturnArgument(int,returnValue));

    QVERIFY(returnValue == 5);
    delete ObjTestSender;
}


void mafEventBusManagerTest::eventNotificationBenchmarkTest() {
    mafString updateID = "GLOBAL_UPDATE_EVENT";
    m_EventBus->logAllEvents();

    QBENCHMARK {
        m_EventBus->notifyEvent(updateID);
    }
}

MAF_REGISTER_TEST(mafEventBusManagerTest);
#include "mafEventBusManagerTest.moc"
