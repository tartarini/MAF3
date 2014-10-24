/*
 *  mafTopicRegistryTest.cpp
 *  mafEventBusTest
 *
 *  Created by Roberto Mucci - Daniele Giunchi on 26/01/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafEventBusTestList.h"

using namespace mafEventBus;

void mafTopicRegistryTest::initTestCase() {
    m_TopicRegistry = mafTopicRegistry::instance();
}

void mafTopicRegistryTest::cleanupTestCase() {
    m_TopicRegistry->shutdown();
}

void mafTopicRegistryTest::mafTopicRegistryRegisterTest() {
    QVERIFY(m_TopicRegistry != NULL);
    QString topic("maf.local.eventBus.testTopic");

    bool result = m_TopicRegistry->registerTopic(topic, this);
    QVERIFY(result);

    result = m_TopicRegistry->registerTopic("maf.local.eventBus.testTopic1", this);
    QVERIFY(result);

    result = m_TopicRegistry->registerTopic("maf.local.eventBus.testTopic2", this);
    QVERIFY(result);

    result = m_TopicRegistry->registerTopic(topic, this);
    QVERIFY(!result);

    result = m_TopicRegistry->registerTopic("", this);
    QVERIFY(!result);

    result = m_TopicRegistry->registerTopic(topic, NULL);
    QVERIFY(!result);

    result = m_TopicRegistry->registerTopic("", NULL);
    QVERIFY(!result);
}

void mafTopicRegistryTest::mafTopicRegistryOwnerTest() {
    QString topic("maf.local.eventBus.testTopic");
    const QObject *obj = m_TopicRegistry->owner(topic);
    QVERIFY(obj == this);

    obj = m_TopicRegistry->owner("");
    QVERIFY(obj == NULL);

    obj = m_TopicRegistry->owner("maf.local.eventBus.TopicNotRegisterd");
    QVERIFY(obj == NULL);

    //Check isPresent() method.
    bool result = m_TopicRegistry->isTopicRegistered(topic);
    QVERIFY(result);

    result = m_TopicRegistry->isTopicRegistered("");
    QVERIFY(!result);

    result = m_TopicRegistry->isTopicRegistered("maf.local.eventBus.TopicNotRegisterd");
    QVERIFY(!result);

    // print 3 topic
    m_TopicRegistry->dump();

    result = m_TopicRegistry->unregisterTopic(topic);
    QVERIFY(result);

    result = m_TopicRegistry->unregisterTopic("maf.local.eventBus.TopicNotRegisterd");
    QVERIFY(!result);

    // print 2 topic
    m_TopicRegistry->dump();
}

#include "mafTopicRegistryTest.moc"



