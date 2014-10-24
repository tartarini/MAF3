/*
 *  mafEventDispatcherRemoteTest.cpp
 *  mafEventBusTest
 *
 *  Created by Daniele Giunchi on 20/04/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafEventBusTestList.h"

using namespace mafEventBus;

void mafEventDispatcherRemoteTest::initTestCase() {
    m_EventDispatcherRemote = new mafEventDispatcherRemote();
}

void mafEventDispatcherRemoteTest::cleanupTestCase() {
    delete m_EventDispatcherRemote;
}

void mafEventDispatcherRemoteTest::mafEventDispatcherRemoteAllocationTest() {
    QVERIFY(m_EventDispatcherRemote != NULL);
}

void mafEventDispatcherRemoteTest::mafEventDispatcherRemoteAccessorsTest() {
    QVERIFY(m_EventDispatcherRemote->networkConnectorClient() == NULL);
    QVERIFY(m_EventDispatcherRemote->networkConnectorServer() == NULL);
}

#include "mafEventDispatcherRemoteTest.moc"



