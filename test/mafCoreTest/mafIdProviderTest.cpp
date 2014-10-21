/*
 *  mafIdProviderTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 22/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

using namespace mafCore;

void mafIdProviderTest::initTestCase() {
    m_IdProvider = mafIdProvider::instance();
}

void mafIdProviderTest::cleanupTestCase() {
    mafMessageHandler::instance()->shutdown();
}

void mafIdProviderTest::createNewIdTest() {
    //! <snippet>
    m_IdValue = m_IdProvider->createNewId();
    //! </snippet>

    mafId id;
    // Named object ID
    id = m_IdProvider->createNewId("TestObjectId");
    QVERIFY(id == m_IdValue + 1);

    // Another named object ID
    id = m_IdProvider->createNewId("TestObjectAnotherId");
    QVERIFY(id == m_IdValue + 2);

    // Duplicated name (not admitted), return the previous defined ID value.
    mafMessageHandler::instance()->testSuiteLogMode(true);
    id = m_IdProvider->createNewId("TestObjectId");
    mafMessageHandler::instance()->testSuiteLogMode(false);
    QVERIFY(id == m_IdValue + 1);
}

void mafIdProviderTest::idNameTest() {
    // No named object ID == OBJECT_ID_XX
    //! <snippet>
    QString name = m_IdProvider->idName(0);
    //! </snippet>
    QCOMPARE(name, QString("OBJECT_ID_0"));

    name = m_IdProvider->idName(m_IdValue + 1);
    QCOMPARE(name, QString("TestObjectId"));

    // not existing id -> return empty string.
    name = m_IdProvider->idName(1500);
    QVERIFY(name.isEmpty());
}

void mafIdProviderTest::idValueTest() {
    mafId id = m_IdProvider->idValue("OBJECT_ID_0");
    QVERIFY(id == 0);

    id = m_IdProvider->idValue("TestObjectId");
    QVERIFY(id == m_IdValue + 1);

    id = m_IdProvider->idValue("NotEists");
    QVERIFY(id == -1);
}

void mafIdProviderTest::setIdNameTest() {
    mafId id;
    id = m_IdProvider->createNewId();

    // Try to change the name with an existing one (should fail)
    bool result = m_IdProvider->setIdName(id, "TestObjectId");
    QVERIFY(result == false);

    // Change name with acceptable string => succeded.
    result = m_IdProvider->setIdName(id, "TestCustomId");
    QVERIFY(result);
}

void mafIdProviderTest::idRemoveTest() {
    // Check for a valid ID
    mafId id = m_IdProvider->idValue("TestCustomId");
    QVERIFY(id != -1);

    // Remove the ID
    m_IdProvider->removeId("TestCustomId");

    // The ID should not be available anymore
    id = m_IdProvider->idValue("TestCustomId");
    QVERIFY(id == -1);
}

#include "mafIdProviderTest.moc"
