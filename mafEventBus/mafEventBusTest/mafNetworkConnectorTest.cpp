/*
 *  mafNetworkConnectorTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 20/04/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafNetworkConnector.h>

using namespace mafEventBus;

//------------------------------------------------------------------------------------------
/**
 Class name: testNetworkConnectorCustom
 This class implements the network connector to be tested.
 */
class  testNetworkConnectorCustom : public  mafNetworkConnector {
    Q_OBJECT

public:
    /// Object constructor.
    testNetworkConnectorCustom();

    /// Create and initialize client
    /*virtual*/ void createClient(mafString hostName, unsigned int port);

    /// Return the string variable initializated and updated from the data pipe.
    /*virtual*/ void createServer(unsigned int port);

    /// Allow to send a network request.
    /*virtual*/ void send(const mafString event_id, mafEventArgumentsList *params);

    /// Start the server.
    /*virtual*/ void startListen();

    /// Return connector status.
    mafString connectorStatus();

    /// retrieve instance of object
    /*virtual*/ mafNetworkConnector *clone();

    /// register all the signals and slots
    /*virtual*/ void initializeForEventBus();

private:
    mafString m_ConnectorStatus; ///< Test Var.
};

mafNetworkConnector *testNetworkConnectorCustom::clone() {
    return new testNetworkConnectorCustom();
}

void testNetworkConnectorCustom::initializeForEventBus() {

}

testNetworkConnectorCustom::testNetworkConnectorCustom() : mafNetworkConnector(), m_ConnectorStatus("") {
}

void testNetworkConnectorCustom::createServer(unsigned int port) {
    m_ConnectorStatus = "Server Created - Port: ";
    m_ConnectorStatus.append(mafString::number(port));
}

void testNetworkConnectorCustom::startListen() {
    m_ConnectorStatus = "Server Listening";
}

void testNetworkConnectorCustom::createClient(mafString hostName, unsigned int port) {
    m_ConnectorStatus = "Client Created - Host: ";
    m_ConnectorStatus.append(hostName);
    m_ConnectorStatus.append(" Port: ");
    m_ConnectorStatus.append(mafString::number(port));
}

void testNetworkConnectorCustom::send(const mafString event_id, mafEventArgumentsList *params) {
    Q_UNUSED(params);
    m_ConnectorStatus = "Event sent with ID: ";
    m_ConnectorStatus.append(event_id);
}

mafString testNetworkConnectorCustom::connectorStatus() {
    return m_ConnectorStatus;
}

//------------------------------------------------------------------------------------------

/**
 Class name: mafNetworkConnectorTest
 This class implements the test suite for mafNetworkConnector.
 */

 //! <title>
//mafNetworkConnector
//! </title>
//! <description>
//mafNetworkConnector is the interface class for client/server objects that
//works over network.
//! </description>


class mafNetworkConnectorTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_NetworkConnector = new testNetworkConnectorCustom();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        if(m_NetworkConnector) delete m_NetworkConnector;
    }

    /// mafNetworkConnector allocation test case.
    void mafNetworkConnectorAllocationTest();
    /// Test the creation of client and server.
    void mafNetworkConnectorCreateClientAndServerTest();

private:
    testNetworkConnectorCustom *m_NetworkConnector; ///< Test var.
};

void mafNetworkConnectorTest::mafNetworkConnectorAllocationTest() {
    QVERIFY(m_NetworkConnector != NULL);
}

void mafNetworkConnectorTest::mafNetworkConnectorCreateClientAndServerTest() {
    mafString res;
    res = "Server Created - Port: 8000";
    m_NetworkConnector->createServer(8000);

    QCOMPARE(m_NetworkConnector->connectorStatus(), res);

    res = "Client Created - Host: localhost Port: 8000";
    m_NetworkConnector->createClient("localhost", 8000);
    QCOMPARE(m_NetworkConnector->connectorStatus(), res);
}

MAF_REGISTER_TEST(mafNetworkConnectorTest);
#include "mafNetworkConnectorTest.moc"

