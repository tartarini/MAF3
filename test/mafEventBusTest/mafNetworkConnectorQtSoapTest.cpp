/*
 *  mafNetworkConnectorQtSoapTest.cpp
 *  mafNetworkConnectorQtSoapTest
 *
 *  Created by Daniele Giunchi on 16/07/10.
 *  Copyright 2009-2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTestSuite.h"
#include <mafNetworkConnectorQtSoap.h>
#include "mafEventDefinitions.h"
#include <mafEventBusManager.h>

#include <QApplication>

#define WSDL_URL "http://localhost:7889/HelloWordService?wsdl"

using namespace mafEventBus;

//-------------------------------------------------------------------------
/**
 Class name: mafObjectCustom
 Custom object needed for testing.
 */
class testObjectCustomForNetworkConnectorSoap : public QObject {
    Q_OBJECT

public:
    /// constructor.
    testObjectCustomForNetworkConnectorSoap();

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

testObjectCustomForNetworkConnectorSoap::testObjectCustomForNetworkConnectorSoap() : m_Var(0) {
}

void testObjectCustomForNetworkConnectorSoap::updateObject() {
    ++m_Var;
}

void testObjectCustomForNetworkConnectorSoap::setObjectValue(int v) {
    m_Var = v;
}


/**
 Class name: mafNetworkConnectorQtSoapTest
 This class implements the test suite for mafNetworkConnectorQtSoap.
 */

//! <title>
//mafNetworkConnectorQtSoap
//! </title>
//! <description>
//mafNetworkConnectorQtSoap provides the connection with soap protocol, the qt class implement only the client
//side. A C++ Server side implementation can be implemented with gSoap library.
//It has been used qxmlrpc library.
//! </description>

class mafNetworkConnectorQtSoapTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        m_EventBus = mafEventBusManager::instance();
        m_NetWorkConnectorQtSoap = new mafEventBus::mafNetworkConnectorQtSoap;
        m_ObjectTest = new testObjectCustomForNetworkConnectorSoap;
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        delete m_ObjectTest;
        delete m_NetWorkConnectorQtSoap;
        m_EventBus->shutdown();
    }

    /// Check the existence of the mafNetworkConnectorQtSoape singleton creation.
    void mafNetworkConnectorQtSoapConstructorTest();

    /// service testing
    void mafNetworkConnectorQtSoapCommunictionPassingStringTest();

    /// service testing
    void mafNetworkConnectorQtSoapCommunictionPassingStringOnAxisServiceTest();

    /// service testing
    void mafNetworkConnectorQtSoapCommunictionPassingStringArrayTest();

    /// gsoap service testing
    void mafNetworkConnectorQtSoapCommunictionWithGSOAPServiceTest();

private:
    mafEventBusManager *m_EventBus; ///< event bus instance
    mafNetworkConnectorQtSoap *m_NetWorkConnectorQtSoap; ///< EventBus test variable instance.
    testObjectCustomForNetworkConnectorSoap *m_ObjectTest;
};

void mafNetworkConnectorQtSoapTest::mafNetworkConnectorQtSoapConstructorTest() {
    QVERIFY(m_NetWorkConnectorQtSoap != NULL);


    //delete and recreate instance in order to cover different branch inside destructor
    delete m_NetWorkConnectorQtSoap;
    m_NetWorkConnectorQtSoap = NULL;
    m_NetWorkConnectorQtSoap = new mafEventBus::mafNetworkConnectorQtSoap;
}


void mafNetworkConnectorQtSoapTest::mafNetworkConnectorQtSoapCommunictionPassingStringTest() {
    //create soap client, initializing host and port
    m_NetWorkConnectorQtSoap->createClient("localhost", 7889);
    m_NetWorkConnectorQtSoap->setWSDL(WSDL_URL);

    // customize call
    mafEventArgumentsList myList; // create list to send

    //inside there is ONE hash which has name, value
    QVariantMap values;

    //this is value 1
    QString valueToSend = "EternoDolore";

    QVariant v(valueToSend);

    //set the name and the value
    values.insert("input", v);


    //append inside the list
    myList.push_back(mafEventArgument(QVariantMap, values));

    // send call
    m_NetWorkConnectorQtSoap->setAction("myEcho");
    m_NetWorkConnectorQtSoap->setPath(WSDL_URL);
    m_NetWorkConnectorQtSoap->send("myEcho", &myList);

    //wait for response from remote server
    QTime dieTime = QTime::currentTime().addSecs(5);
    while(QTime::currentTime() < dieTime) {
       QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
    }

    // compare results
    QtSoapType *soapTypeResult = m_NetWorkConnectorQtSoap->response();
    if(soapTypeResult)
    qDebug("%s", soapTypeResult->toString().toLatin1().constData());
}

void mafNetworkConnectorQtSoapTest::mafNetworkConnectorQtSoapCommunictionPassingStringOnAxisServiceTest() {
    //create soap client, initializing host and port
    m_NetWorkConnectorQtSoap->createClient("localhost", 8280);
    m_NetWorkConnectorQtSoap->setWSDL("http://localhost:8280/services/echo?wsdl");

    // customize call
    mafEventArgumentsList myList; // create list to send

    //inside there is ONE hash which has name, value
    QVariantMap values;

    //this is value 1
    QString valueToSend = "IBM";

    QVariant v(valueToSend);

    //set the name and the value
    values.insert("in", v);


    //append inside the list
    myList.push_back(mafEventArgument(QVariantMap,values));

    QtSoapNamespaces &registry = QtSoapNamespaces::instance();
    registry.registerNamespace("ns", "http://echo.services.core.carbon.wso2.org");

    // send call
    m_NetWorkConnectorQtSoap->registerNamespace("ns", "http://echo.services.core.carbon.wso2.org");
    m_NetWorkConnectorQtSoap->setAction("urn:echoString");
    m_NetWorkConnectorQtSoap->setPath("http://localhost:8280/services/echo.echoHttpSoap11Endpoint");
    m_NetWorkConnectorQtSoap->send("ns:echoString", &myList);

    //wait for response from remote server
    QTime dieTime = QTime::currentTime().addSecs(5);
    while(QTime::currentTime() < dieTime) {
       QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
    }

    // compare results
    QtSoapType *soapTypeResult = m_NetWorkConnectorQtSoap->response();
    if(soapTypeResult)
    qDebug("%s", soapTypeResult->toString().toLatin1().constData());
}

void mafNetworkConnectorQtSoapTest::mafNetworkConnectorQtSoapCommunictionPassingStringArrayTest() {
    //create soap client, initializing host and port
    m_NetWorkConnectorQtSoap->createClient("localhost", 7889);
    m_NetWorkConnectorQtSoap->setWSDL(WSDL_URL);

    // customize call
    mafEventArgumentsList myList; // create list to send

    //inside there is ONE hash which has name, value
    QVariantMap values;

    //this is value 1
    QList<QString> list1;
    list1.push_back("eventA1");
    list1.push_back("eventA2");

    //this is value 2
    QList<QString> list2;
    list2.push_back("dataA1");
    list2.push_back("dataA2");

    //incapsulate inside a QVariant
    QVariant v1(list1);
    QVariant v2(list2);

    //set the name and the value
    values.insert("arrEvent", v1);
    //set the name and the value
    values.insert("arrData", v2);

    //append inside the list
    myList.push_back(mafEventArgument(QVariantMap,values));

    // send call
    m_NetWorkConnectorQtSoap->setAction("testArray");
    m_NetWorkConnectorQtSoap->setPath(WSDL_URL);
    m_NetWorkConnectorQtSoap->send("testArray", &myList);

    //wait for response from remote server
    QTime dieTime = QTime::currentTime().addSecs(5);
    while(QTime::currentTime() < dieTime) {
       QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
    }

    // compare results
    QtSoapType *soapTypeResult = m_NetWorkConnectorQtSoap->response();
    if(soapTypeResult)
    qDebug("%s", soapTypeResult->toString().toLatin1().constData());
}

void mafNetworkConnectorQtSoapTest::mafNetworkConnectorQtSoapCommunictionWithGSOAPServiceTest() {
    //create soap client, initializing host and port
    m_NetWorkConnectorQtSoap->createClient("ws.biomedtown.org", 80);
    m_NetWorkConnectorQtSoap->setWSDL("http://ws.biomedtown.org/hello.wsdl");

    // customize call
    mafEventArgumentsList myList; // create list to send

    //inside there is ONE hash which has name, value
    QVariantMap values;
    values.insert("input", "stringTest");

    //append inside the list
    myList.push_back(mafEventArgument(QVariantMap,values));

    // send call
    m_NetWorkConnectorQtSoap->registerNamespace("h", "http://tempuri.org/h.xsd");
    m_NetWorkConnectorQtSoap->setAction("");
    m_NetWorkConnectorQtSoap->setPath("hello.cgi"); //warning : the path of the service should be selected depending in which way the service is implemented.
    m_NetWorkConnectorQtSoap->send("h:hello", &myList);

    //wait for response from remote server
    QTime dieTime = QTime::currentTime().addSecs(5);
    while(QTime::currentTime() < dieTime) {
       QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
    }

    // compare results
    QtSoapType *soapTypeResult = m_NetWorkConnectorQtSoap->response();
    if(soapTypeResult)
    qDebug("%s", soapTypeResult->toString().toLatin1().constData());
}

MAF_REGISTER_TEST(mafNetworkConnectorQtSoapTest);
#include "mafNetworkConnectorQtSoapTest.moc"
