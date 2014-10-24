
/*
 *  mafEventTest.cpp
 *  mafEventBusTest
 *
 *  Created by Daniele Giunchi on 20/04/10.
 *  Copyright 2009-2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafEventBusTestList.h"

using namespace mafEventBus;

void mafEventTest::initTestCase() {
    m_Event = new mafEvent();
}

void mafEventTest::cleanupTestCase() {
    delete m_Event;
}

void mafEventTest::mafEventAllocationTest() {
    QVERIFY(m_Event != NULL);
}

void mafEventTest::mafEventAccessorsTest() {
    m_Event->setEventType(mafEventTypeRemote);
    QVERIFY(m_Event->eventType() == mafEventTypeRemote);
    QVERIFY(m_Event->isEventLocal() != true);

    QString topic = "maf.level1.level2.level3";
    m_Event->setEventTopic(topic);
    QCOMPARE(m_Event->eventTopic(), topic);

    const QVariantHash *cdict = m_Event->entries(); //const metod
    QVERIFY(cdict != NULL);

    QVariantHash *dict = m_Event->entries();
    QVERIFY(dict != NULL);

    QVariant var = (*m_Event)[TOPIC];
    QString check = var.toString();
    QCOMPARE(check, topic);

    delete m_Event;
    topic = "maf.lev1.lev2.lev3";
    m_Event = new mafEvent(topic, mafEventTypeLocal, mafSignatureTypeSignal, this, "testmethod");
    QCOMPARE(m_Event->eventTopic(), topic);
}

void mafEventTest::mafEventArgListRetValTest() {
    QString topic = "maf.level1.level2.level3";
    mafEvent *ev = new mafEvent(topic);
    int par1(5);
    QObject par2;
    ev->addParameter(mafEventArgument(int, par1));
    ev->addParameter(mafEventArgument(QObject*, &par2));
    mafEventArgumentsListPointer argList = ev->argumentList();
    int n = argList->count();
    QVERIFY(2 == n);
    
    delete ev;
}

#include "mafEventTest.moc"
