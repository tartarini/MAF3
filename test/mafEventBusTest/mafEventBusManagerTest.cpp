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
#include <mafNetworkConnector.h>

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

    /// register a custom callback
    void registerCustomCallback();

public Q_SLOTS:
    /// Test slot that will increment the value of m_Var when an UPDATE_OBJECT event is raised.
    void updateObject();
    void setObjectValue(int v);
    int returnObjectValue();

Q_SIGNALS:
    void valueModified(int v);
    void objectModified();
    int returnObjectValueSignal();

private:
    int m_Var; ///< Test var.
};

testObjectCustom::testObjectCustom() : m_Var(0) {
}

void testObjectCustom::updateObject() {
    ++m_Var;
}

void testObjectCustom::setObjectValue(int v) {
    m_Var = v;
}

int testObjectCustom::returnObjectValue() {
    int value = 5;
    return value;
}

void testObjectCustom::registerCustomCallback() {
    mafRegisterLocalCallback("maf.local.custom.topic", this, "updateObject()");
}

//------------------------------------------------------------------------------------------
/**
 Class name: testNetworkConnectorForEventBus
 This class implements the network connector to be tested.
 */
class  testNetworkConnectorForEventBus : public  mafNetworkConnector {
    Q_OBJECT

public:
    /// Object constructor.
    testNetworkConnectorForEventBus();

    /// Create and initialize client
    /*virtual*/ void createClient(const QString hostName, const unsigned int port, QMap<QString,QVariant> * advancedParameters = NULL);

    /// Return the string variable initializated and updated from the data pipe.
    /*virtual*/ void createServer(const unsigned int port);

    /// Allow to send a network request.
    /*virtual*/ void send(const QString event_id, mafEventArgumentsList *params, bool externalSend = false);

    /// Start the server.
    /*virtual*/ void startListen();

    /// Return connector status.
    QString connectorStatus();

    /// retrieve instance of object
    /*virtual*/ mafNetworkConnector *clone();

    /// register all the signals and slots
    /*virtual*/ void initializeForEventBus();

private:
    QString m_ConnectorStatus; ///< Test Var.
};

mafNetworkConnector *testNetworkConnectorForEventBus::clone() {
    return new testNetworkConnectorForEventBus();
}

void testNetworkConnectorForEventBus::initializeForEventBus() {
}

testNetworkConnectorForEventBus::testNetworkConnectorForEventBus() : mafNetworkConnector(), m_ConnectorStatus("") {
     m_Protocol = "FakeProtocol";
}

void testNetworkConnectorForEventBus::createServer(const unsigned int port) {
    m_ConnectorStatus = "Server Created - Port: ";
    m_ConnectorStatus.append(QString::number(port));
}

void testNetworkConnectorForEventBus::startListen() {
    m_ConnectorStatus = "Server Listening";
}

void testNetworkConnectorForEventBus::createClient(const QString hostName, const unsigned int port, QMap<QString,QVariant> * advancedParameters) {
    m_ConnectorStatus = "Client Created - Host: ";
    m_ConnectorStatus.append(hostName);
    m_ConnectorStatus.append(" Port: ");
    m_ConnectorStatus.append(QString::number(port));
}

void testNetworkConnectorForEventBus::send(const QString event_id, mafEventArgumentsList *params, bool externalSend) {
    Q_UNUSED(params);
    Q_UNUSED(externalSend);
    m_ConnectorStatus = "Event sent with ID: ";
    m_ConnectorStatus.append(event_id);
}

QString testNetworkConnectorForEventBus::connectorStatus() {
    return m_ConnectorStatus;
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

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        m_EventBus = mafEventBusManager::instance();
        m_ObjTestObserver = new testObjectCustom();
        m_ObjTestObserver2 = new testObjectCustom();
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        delete m_ObjTestObserver;
        delete m_ObjTestObserver2;
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
    mafEventBusManager *m_EventBus; ///< EventBus test variable instance.
};

void mafEventBusManagerTest::mafEventBusManagerConstructorTest() {
    QVERIFY(m_EventBus != NULL);
}

void mafEventBusManagerTest::eventBusRegistrationNotificationTest() {
    int status = m_ObjTestObserver->var();
    QVERIFY(status == 0);

    // Create new Event ID used for callback and event notification.
    QString updateID = "maf.local.eventBus.globalUpdate";
    mafRegisterLocalCallback(updateID, m_ObjTestObserver, "updateObject()");

    // Register also the second test observer to the global update event
    mafRegisterLocalCallback(updateID, m_ObjTestObserver2, "updateObject()");

    // Notify the update event... (event logging is disabled)
    m_EventBus->notifyEvent("maf.local.eventBus.globalUpdate");

    status = m_ObjTestObserver->var();
    QVERIFY(status == 1);

    // Notify again but with event logging enabled...
    m_EventBus->enableEventLogging();
    m_EventBus->notifyEvent(updateID);

    // ... and now filter events only with ID == "CUSTOM_SIGNAL"
    //mafId customID = idProvider->createNewId("CUSTOM_SIGNAL");
    QString customID = "CUSTOM_SIGNAL";

    // ...and enable event logging for that id.
    m_EventBus->logEventTopic(customID);

    // Previous ID notification should be skipped by logger (so only one log in the console)
    m_EventBus->notifyEvent(updateID);
}

 void mafEventBusManagerTest::eventBusWithArgumentTest() {
    testObjectCustom *ObjTestSender = new testObjectCustom();
    ObjTestSender->setObjectValue(52);

    // Create new Event ID used for callback and event notification.
    QString setValueID = "SETVALUE_SIGNAL";
    mafRegisterLocalSignal(setValueID, ObjTestSender, "valueModified(int)");

    // Register the callback to update the object custom:
    mafRegisterLocalCallback(setValueID, m_ObjTestObserver, "setObjectValue(int)");
    // Register also the second observer...
    mafRegisterLocalCallback(setValueID, m_ObjTestObserver2, "setObjectValue(int)");

    //! <snippet>
    mafEventArgumentsList list;
    list.append(mafEventArgument(int, ObjTestSender->var()));

    m_EventBus->notifyEvent(setValueID, mafEventTypeLocal, &list);
    //! </snippet>
    int status = m_ObjTestObserver->var();
    QVERIFY(status == ObjTestSender->var());
    delete ObjTestSender;
    mafEventBusManager::instance()->removeSignal(ObjTestSender, setValueID);
}

void mafEventBusManagerTest::eventBusWithReturnArgumentTest() {
    testObjectCustom *ObjTestSender = new testObjectCustom();
    ObjTestSender->setObjectValue(52);

    // Create new Event ID used for callback and event notification.
    QString returnValueID = "RETURNVALUE_SIGNAL";
    mafRegisterLocalSignal(returnValueID, ObjTestSender, "returnObjectValueSignal()");
    mafRegisterLocalCallback(returnValueID, ObjTestSender, "returnObjectValue()");


    //Notify event with return argument
    int returnValue = 0;
    QGenericReturnArgument ret_val = mafEventReturnArgument(int,returnValue);
    m_EventBus->notifyEvent(returnValueID, mafEventTypeLocal, NULL, &ret_val);

    QVERIFY(returnValue == 5);
    delete ObjTestSender;
}


void mafEventBusManagerTest::eventNotificationBenchmarkTest() {
    QString updateID = "maf.local.eventBus.globalUpdate";
    m_EventBus->logAllEvents();

    QBENCHMARK {
        m_EventBus->notifyEvent(updateID);
    }
}

void mafEventBusManagerTest::remoteConnectionTest() {
    m_EventBus->createServer("XMLRPC", 8000);
    m_EventBus->startListen();

    m_EventBus->createClient("XMLRPC", "localhost", 8000);

    //create list to send from the client
    //first parameter is a list which contains event properties
    QVariantList eventParameters;
    eventParameters.append("maf.local.eventBus.globalUpdate");
    eventParameters.append(mafEventTypeLocal);
    eventParameters.append(mafSignatureTypeCallback);
    eventParameters.append("updateObject()");

    QVariantList dataParameters;

    mafEventArgumentsList listToSend;
    listToSend.append(mafEventArgument(QVariantList, eventParameters));
    listToSend.append(mafEventArgument(QVariantList, dataParameters));

    //with eventbus
    QString topic = "maf.remote.eventBus.communication.send.xmlrpc";
    m_EventBus->notifyEvent(topic, mafEventTypeRemote , &listToSend);

    QTime dieTime = QTime::currentTime().addSecs(3);
    while(QTime::currentTime() < dieTime) {
       QCoreApplication::processEvents(QEventLoop::AllEvents, 3);
    }
}

void mafEventBusManagerTest::plugNetworkConnectorTest() {
    m_EventBus->plugNetworkConnector("CUSTOM_CONNECTOR", NULL);
}

void mafEventBusManagerTest::isLocalSignalPresentTest() {
    QVERIFY(m_EventBus->isLocalSignalPresent("maf.paf.naf.daf") == false);
}

void mafEventBusManagerTest::reverseOrderRegistrationTest() {
    testObjectCustom *ObjTestSender = new testObjectCustom();
    int startValue = 42;
    m_ObjTestObserver->setObjectValue(startValue);

    //register a custom callback
    m_ObjTestObserver->registerCustomCallback();

    //register custom signal
    mafRegisterLocalSignal("maf.local.custom.topic", ObjTestSender, "objectModified()");

    //notify event
    m_EventBus->notifyEvent("maf.local.custom.topic");

    ++startValue; //notify will update the value adding 1
    int check = m_ObjTestObserver->var();
    QVERIFY(startValue == check);
    delete ObjTestSender;
}


MAF_REGISTER_TEST(mafEventBusManagerTest);
#include "mafEventBusManagerTest.moc"
