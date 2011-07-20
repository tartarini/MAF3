/*
 *  mafNetworkConnectorRESTTest.cpp
 *  mafNetworkConnectorRESTTest
 *
 *  Created by Daniele Giunchi on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafNetworkConnectorREST.h>
#include <mafEventBusManager.h>

#include <QApplication>

using namespace mafEventBus;
using namespace mafCore;

//-------------------------------------------------------------------------
/**
 Class name: mafObjectCustom
 Custom object needed for testing.
 */
class testObjectCustomForNetworkConnectorREST : public QObject {
    Q_OBJECT

public:
    /// constructor.
    testObjectCustomForNetworkConnectorREST();

    /// Return tha var's value.
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

testObjectCustomForNetworkConnectorREST::testObjectCustomForNetworkConnectorREST() : m_Var(0) {
}

void testObjectCustomForNetworkConnectorREST::updateObject() {
    ++m_Var;
}

void testObjectCustomForNetworkConnectorREST::setObjectValue(int v) {
    m_Var = v;
}


/**
 Class name: mafNetworkConnectorRESTTest
 This class implements the test suite for mafNetworkConnectorREST.
 */

//! <title>
//mafNetworkConnectorREST
//! </title>
//! <description>
//mafNetworkConnectorREST provides the connection with REST protocol.
//It has been used qxmlrpc library.
//! </description>

class mafNetworkConnectorRESTTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_EventBus = mafEventBusManager::instance();
        m_NetWorkConnectorQREST = new mafEventBus::mafNetworkConnectorREST();
        m_ObjectTest = new testObjectCustomForNetworkConnectorREST();
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        if(m_ObjectTest) {
            delete m_ObjectTest;
            m_ObjectTest = NULL;
        }
        delete m_NetWorkConnectorQREST;
        m_EventBus->shutdown();
    }

    /// Check the existence of the mafNetworkConnectorRESTe singletone creation.
    void mafNetworkConnectorRESTConstructorTest();

    /// Check the existence of the mafNetworkConnectorRESTe singletone creation.
    void mafNetworkConnectorRESTCommunictionTest();

private:
    mafEventBusManager *m_EventBus; ///< event bus instance
    mafNetworkConnectorREST *m_NetWorkConnectorQREST; ///< EventBus test variable instance.
    testObjectCustomForNetworkConnectorREST *m_ObjectTest;
};

void mafNetworkConnectorRESTTest::mafNetworkConnectorRESTConstructorTest() {
    QVERIFY(m_NetWorkConnectorQREST != NULL);
}


void mafNetworkConnectorRESTTest::mafNetworkConnectorRESTCommunictionTest() {
    /*m_NetWorkConnectorQREST->createServer(8000);
    m_NetWorkConnectorQREST->startListen();


    // Register callback (done by the remote object).
    mafRegisterLocalCallback("maf.local.eventBus.globalUpdate", m_ObjectTest, "updateObject()");

    //m_EventBus->createClient("REST", "localhost", 8000);
    m_NetWorkConnectorQREST->createClient("localhost", 8000);

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

    m_NetWorkConnectorQREST->send("maf.remote.eventBus.communication.send.xmlrpc", &listToSend);


    QTime dieTime = QTime::currentTime().addSecs(3);
    while(QTime::currentTime() < dieTime) {
       QCoreApplication::processEvents(QEventLoop::AllEvents, 3);
    }*/
}

MAF_REGISTER_TEST(mafNetworkConnectorRESTTest);
#include "mafNetworkConnectorRESTTest.moc"

