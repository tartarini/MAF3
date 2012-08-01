/*
 *  mafNetworkConnectorQXMLRPCTest.cpp
 *  mafNetworkConnectorQXMLRPCTest
 *
 *  Created by Daniele Giunchi on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafNetworkConnectorQXMLRPC.h>
#include <mafEventBusManager.h>

#include <QApplication>

using namespace mafEventBus;
using namespace mafCore;

//-------------------------------------------------------------------------
/**
 Class name: mafObjectCustom
 Custom object needed for testing.
 */
class testObjectCustomForNetworkConnectorXMLRPC : public QObject {
    Q_OBJECT

public:
    /// constructor.
    testObjectCustomForNetworkConnectorXMLRPC();

    /// Return tha var's value.
    int var() {return m_Var;}

public Q_SLOTS:
    /// Test slot that will increment the value of m_Var when an UPDATE_OBJECT event is raised.
    void updateObject();
    void setObjectValue(int v);

    void consumeResponse(QVariant response);

Q_SIGNALS:
    void valueModified(int v);
    void objectModified();

private:
    int m_Var; ///< Test var.
};

testObjectCustomForNetworkConnectorXMLRPC::testObjectCustomForNetworkConnectorXMLRPC() : m_Var(0) {
}

void testObjectCustomForNetworkConnectorXMLRPC::updateObject() {
    ++m_Var;
}

void testObjectCustomForNetworkConnectorXMLRPC::setObjectValue(int v) {
    m_Var = v;
}

void testObjectCustomForNetworkConnectorXMLRPC::consumeResponse(QVariant response) {
    qDebug() << "Consuming response from https request ...";
}

/**
 Class name: mafNetworkConnectorQXMLRPCTest
 This class implements the test suite for mafNetworkConnectorQXMLRPC.
 */

//! <title>
//mafNetworkConnectorQXMLRPC
//! </title>
//! <description>
//mafNetworkConnectorQXMLRPC provides the connection with xml-rpc protocol.
//It has been used qxmlrpc library.
//! </description>

class mafNetworkConnectorQXMLRPCTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        m_EventBus = mafEventBusManager::instance();
        m_NetWorkConnectorQXMLRPC = new mafEventBus::mafNetworkConnectorQXMLRPC();
        m_ObjectTest = new testObjectCustomForNetworkConnectorXMLRPC();
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        if(m_ObjectTest) {
            delete m_ObjectTest;
            m_ObjectTest = NULL;
        }
        delete m_NetWorkConnectorQXMLRPC;
        m_EventBus->shutdown();
    }

    /// Check the existence of the mafNetworkConnectorQXMLRPCe singletone creation.
    void mafNetworkConnectorQXMLRPCConstructorTest();

    /// Check the existence of the mafNetworkConnectorQXMLRPCe singletone creation.
    void mafNetworkConnectorQXMLRPCCommunictionTest();

    /// Check NULL for the mafNetworkConnectorQXMLRPCe.
    void mafNetworkConnectorQXMLRPCCommunictionNullTest();

    /// Check the communication of the mafNetworkConnectorQXMLRPCe with an header QMap as third parameter.
    void mafNetworkConnectorQXMLRPCCommunictionWithHeaderMapTest();

    /// Check the communication of the mafNetworkConnectorQXMLRPCe using https
    void httpsTest();

    /// Check the communication of the mafNetworkConnectorQXMLRPCe using https without parameters
    void httpsWithoutParametersTest();
    
private:
    mafEventBusManager *m_EventBus; ///< event bus instance
    mafNetworkConnectorQXMLRPC *m_NetWorkConnectorQXMLRPC; ///< EventBus test variable instance.
    testObjectCustomForNetworkConnectorXMLRPC *m_ObjectTest;
};

void mafNetworkConnectorQXMLRPCTest::mafNetworkConnectorQXMLRPCConstructorTest() {
    QVERIFY(m_NetWorkConnectorQXMLRPC != NULL);
}


void mafNetworkConnectorQXMLRPCTest::mafNetworkConnectorQXMLRPCCommunictionTest() {
    m_NetWorkConnectorQXMLRPC->createServer(8000);
    m_NetWorkConnectorQXMLRPC->startListen();


    // Register callback (done by the remote object).
    mafRegisterLocalCallback("maf.local.eventBus.globalUpdate", m_ObjectTest, "updateObject()");

    //m_EventBus->createClient("XMLRPC", "localhost", 8000);
    m_NetWorkConnectorQXMLRPC->createClient("localhost", 8000);

    //create list to send from the client
    //first parameter is a list which contains event prperties


    QVariantList eventParameters;
    eventParameters.append("maf.local.eventBus.globalUpdate");
    eventParameters.append(mafEventTypeLocal);
    eventParameters.append(mafSignatureTypeCallback);
    eventParameters.append("updateObject()");

    QVariantList dataParameters;

    mafEventArgumentsList listToSend;
    listToSend.append(mafEventArgument(QVariantList, eventParameters));
    listToSend.append(mafEventArgument(QVariantList, dataParameters));

    m_NetWorkConnectorQXMLRPC->send("maf.remote.eventBus.communication.send.xmlrpc", &listToSend);


    QTime dieTime = QTime::currentTime().addSecs(3);
    while(QTime::currentTime() < dieTime) {
       QCoreApplication::processEvents(QEventLoop::AllEvents, 3);
    }
}

void mafNetworkConnectorQXMLRPCTest::mafNetworkConnectorQXMLRPCCommunictionNullTest() {

    m_NetWorkConnectorQXMLRPC->send("maf.remote.eventBus.communication.send.xmlrpc", NULL);

    QTime dieTime = QTime::currentTime().addSecs(3);
    while(QTime::currentTime() < dieTime) {
       QCoreApplication::processEvents(QEventLoop::AllEvents, 3);
    }
}


void mafNetworkConnectorQXMLRPCTest::mafNetworkConnectorQXMLRPCCommunictionWithHeaderMapTest() {
    //use the same client server of the previous test
    
    // Register callback (done by the remote object).
    mafRegisterLocalCallback("maf.local.eventBus.globalUpdate", m_ObjectTest, "updateObject()");

    //Add authentication map
    QMap<QString, QVariant> authenticationMap;
    authenticationMap.insert("coockie", "testCookie");

    m_NetWorkConnectorQXMLRPC->setAdvancedParameters(&authenticationMap);
	
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

    m_NetWorkConnectorQXMLRPC->send("maf.remote.eventBus.communication.send.xmlrpc", &listToSend);


    QTime dieTime = QTime::currentTime().addSecs(3);
    while(QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 3);
    }
}

void mafNetworkConnectorQXMLRPCTest::httpsTest() {
    if (m_NetWorkConnectorQXMLRPC) {
        delete m_NetWorkConnectorQXMLRPC;
    }
    mafNetworkConnectorQXMLRPC *m_NetWorkConnectorQXMLRPC = new mafEventBus::mafNetworkConnectorQXMLRPC;

    QMap<QString, QVariant> advancedParameters;
    advancedParameters.insert("connectionmode", 1);

    QString serviceURL("www.physiomespace.com");
    //serviceURL.append("/?ticket=");
    //serviceURL.append(m_ConnectionInfo["sessionTicket"]);
    // Prepare to submit request.
    m_NetWorkConnectorQXMLRPC->createClient(serviceURL, 443, &advancedParameters);
    
    
    mafEventArgumentsList myList; // create list to send
	//inside there is ONE hash which has name, value

	QVariantMap values;
        
    //ticket for poors
	QVariant v0("");
	//set the name and the value
	values.insert("sessionTicket", v0);
        
    //criteria
	QVariant v1(".cdb");
	//set the name and the value
	values.insert("criteria", v1);

    //criteria
	QVariant v2("averageQuadraticDistance");
	//set the name and the value
	values.insert("similarityFunction", v2);

    //criteria
	QVariant v3("data_12345");
	//set the name and the value
	values.insert("dataresource", v3);
        
	//append inside the list
	myList.push_back(mafEventArgument(QVariantMap,values));
        
	// register respose callback
    QObject::connect(m_NetWorkConnectorQXMLRPC, SIGNAL(updatedResponseSignal(QVariant)), m_ObjectTest, SLOT(consumeResponse(QVariant)), Qt::DirectConnection);

	// send call 
    bool externalSend = true;
	m_NetWorkConnectorQXMLRPC->send("searchService", &myList, externalSend);
        
	// wait for response consumtion, but handle server timeout
        
	QEventLoop eventLoop;
    QTimer timer;
	QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()), Qt::DirectConnection);
        
	// set timeout to 10 minutes
	timer.setSingleShot(true);
	timer.start( 10000 );
        
    eventLoop.exec();
        
}

void mafNetworkConnectorQXMLRPCTest::httpsWithoutParametersTest() {
    if (m_NetWorkConnectorQXMLRPC) {
        delete m_NetWorkConnectorQXMLRPC;
    }
    mafNetworkConnectorQXMLRPC *m_NetWorkConnectorQXMLRPC = new mafEventBus::mafNetworkConnectorQXMLRPC;

    QMap<QString, QVariant> advancedParameters;
    advancedParameters.insert("connectionmode", 1);

    QString serviceURL("www.physiomespace.com");
    //serviceURL.append("/?ticket=");
    //serviceURL.append(m_ConnectionInfo["sessionTicket"]);
    // Prepare to submit request.
    m_NetWorkConnectorQXMLRPC->createClient(serviceURL, 443, &advancedParameters);


    // register respose callback
    QObject::connect(m_NetWorkConnectorQXMLRPC, SIGNAL(updatedResponseSignal(QVariant)), m_ObjectTest, SLOT(consumeResponse(QVariant)), Qt::DirectConnection);

    // send call
    bool externalSend = true;
    //nome servizio = "searchService", chiedere a Matteo il nome del servizio nuovo
    m_NetWorkConnectorQXMLRPC->send("searchService", NULL, externalSend);

    // wait for response consumtion, but handle server timeout

    QEventLoop eventLoop;
    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()), Qt::DirectConnection);

    // set timeout to 10 minutes
    timer.setSingleShot(true);
    timer.start( 10000 );

    eventLoop.exec();

}


MAF_REGISTER_TEST(mafNetworkConnectorQXMLRPCTest);
#include "mafNetworkConnectorQXMLRPCTest.moc"

