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

    /// Check the communication of the mafNetworkConnectorQXMLRPCe with an header QMap as third parameter.
    void mafNetworkConnectorQXMLRPCCommunictionWithHeaderMapTest();

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

void mafNetworkConnectorQXMLRPCTest::mafNetworkConnectorQXMLRPCCommunictionWithHeaderMapTest() {
    m_NetWorkConnectorQXMLRPC->createServer(8001);
    m_NetWorkConnectorQXMLRPC->startListen();


    // Register callback (done by the remote object).
    mafRegisterLocalCallback("maf.local.eventBus.globalUpdate", m_ObjectTest, "updateObject()");

    //m_EventBus->createClient("XMLRPC", "localhost", 8000);
    m_NetWorkConnectorQXMLRPC->createClient("localhost", 8001);

    //Add authentication map
    QMap<QString, QString> authenticationMap;
    authenticationMap.insert("coockie", "testCookie");

    m_NetWorkConnectorQXMLRPC->setAuthenticationHeader(&authenticationMap);


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

MAF_REGISTER_TEST(mafNetworkConnectorQXMLRPCTest);
#include "mafNetworkConnectorQXMLRPCTest.moc"

