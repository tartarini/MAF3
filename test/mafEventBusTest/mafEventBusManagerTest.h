/*
 *  mafEventBusManagerTest.h
 *  mafEventBusManagerTest
 *
 *  Created by Daniele Giunchi on 09/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafEventBusManager.h>
#include <mafEventDefinitions.h>
#include <mafNetworkConnector.h>

class testObjectCustom;
class testNetworkConnectorForEventBus;

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

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

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

    /// Test eventbus with remote connection (xmlrpc test)
    void remoteConnectionTest();

    /// test plugNetworkConnector
    void plugNetworkConnectorTest();

    /// test method for check if the signal is present.
    void isLocalSignalPresentTest();

    /// test registration reversing the order of signal and callback
    void reverseOrderRegistrationTest();

private:
    testObjectCustom *m_ObjTestObserver; ///< Test variable.
    testObjectCustom *m_ObjTestObserver2; ///< Test variable.
    mafEventBus::mafEventBusManager *m_EventBus; ///< EventBus test variable instance.
};
