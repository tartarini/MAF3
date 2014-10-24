/*
 *  mafTopicRegistryTest.h
 *  mafEventBusTest
 *
 *  Created by Daniele Giunchi on 09/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafTopicRegistry.h>

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
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafTopicRegistry registration test case.
    void mafTopicRegistryRegisterTest();

    /// mafTopicRegistry owner test case.
    void mafTopicRegistryOwnerTest();

private:
    mafEventBus::mafTopicRegistry *m_TopicRegistry; ///< Test var.
};
