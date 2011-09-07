/*
 *  mafTopicRegistryTest.cpp
 *  mafEventBusTest
 *
 *  Created by Roberto Mucci on 26/01/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafTopicRegistry.h>

using namespace mafEventBus;

/**
 Class name: mafTopicRegistryTest
 This class implements the test suite for mafTopicRegistry.
 */

//! <title>
//mafTopicRegistry
//! </title>
//! <description>
//mafTopicRegistry provides the registration of topic and topic owner in a hash.
//! </description>

class mafTopicRegistryTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        m_TopicRegistry = mafTopicRegistry::instance();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        m_TopicRegistry->shutdown();
    }

    /// mafTopicRegistry registration test case.
    void mafTopicRegistryRegisterTest();

    /// mafTopicRegistry owner test case.
    void mafTopicRegistryOwnerTest();


private:
    mafTopicRegistry *m_TopicRegistry; ///< Test var.

};

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


MAF_REGISTER_TEST(mafTopicRegistryTest);
#include "mafTopicRegistryTest.moc"



