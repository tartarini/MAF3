/*
 *  mafEventBusManagerTest.cpp
 *  mafEventBusManagerTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 27/03/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafEventBusTestList.h"

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

void mafEventBusManagerTest::initTestCase() {
    m_EventBus = mafEventBusManager::instance();
    m_ObjTestObserver = new testObjectCustom();
    m_ObjTestObserver2 = new testObjectCustom();
}

void mafEventBusManagerTest::cleanupTestCase() {
    delete m_ObjTestObserver;
    delete m_ObjTestObserver2;
    m_EventBus->shutdown();
}

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

    // ------------------ DEPRECATED CODE (commented) ------------------
//    mafEventArgumentsList list;
//    list.append(mafEventArgument(int, ObjTestSender->var()));
//
//    m_EventBus->notifyEvent(setValueID, mafEventTypeLocal, &list);
    
    // ------------------ Event notification with arguments ------------------
    //! <snippet>
    mafEvent ev(setValueID);
    ev.addParameter(mafEventArgument(int, ObjTestSender->var()));
    
    m_EventBus->notifyEvent(ev);
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


    int returnValue = 0;
    // ------------------ DEPRECATED CODE (commented) ------------------
//    QGenericReturnArgument ret_val = mafEventReturnArgument(int,returnValue);
//    m_EventBus->notifyEvent(returnValueID, mafEventTypeLocal, NULL, &ret_val);

    //Notify event with return argument
    mafEvent ev(returnValueID);
    ev.setReturnValue(mafEventReturnArgument(int,returnValue));
    m_EventBus->notifyEvent(ev);

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
    /*m_EventBus->createServer("XMLRPC", 8000);
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
    }*/
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

#include "mafEventBusManagerTest.moc"
