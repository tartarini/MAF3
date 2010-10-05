/*
 *  mafIdProviderTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>

using namespace mafCore;

/**
 Class name: mafIdProviderTest
 This class implements the test suite for mafIdProvider.
 */

//! <title>
//mafIdProvider
//! </title>
//! <description>
//mafIdProvider is a singleton which provides the generation of unique ID used for objects or events.
//
//It defines methods to assign a unique ID to objects and events:
//"mafId newObjectId(const mafString id_name = "")" return next valid Id to assign to an object or to use for an event.
//By default, if no name is given to the ID, a OBJECT_ID name is assigned.
//
//The association between IDs and corresponding name is stored in a mafIdHashType (typedef mafHash<mafId, mafString> mafIdHashType)
//! </description>

class mafIdProviderTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        m_IdProvider = mafIdProvider::instance();
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
    }

    /// Test the ID generation
    void createNewIdTest();
    /// Test the change of ID name
    void setIdNameTest();
    /// idName test case
    void idNameTest();
    /// idValue test case
    void idValueTest();

private:
    mafId m_IdValue; ///< Id created
    mafIdProvider *m_IdProvider; ///< Test variable
};

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
    mafString name = m_IdProvider->idName(0);
    //! </snippet>
    QCOMPARE(name, mafString("OBJECT_ID_0"));

    name = m_IdProvider->idName(m_IdValue + 1);
    QCOMPARE(name, mafString("TestObjectId"));

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

MAF_REGISTER_TEST(mafIdProviderTest);
#include "mafIdProviderTest.moc"
