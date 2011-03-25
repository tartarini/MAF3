/*
 *  mafResourceTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafMementoResource.h>
#include <mafResource.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

/**
 Class name: mafResourceTest
 This class implements the test suite for mafResource.
 */

//! <title>
//mafResource
//! </title>
//! <description>
//mafResource is the base resource class for MAF3.
//It provides basic API to set input and get output from a mafResource.
//! </description>

class mafResourceTest: public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafEventBusManager::instance();
        m_Resource = mafNEW(mafResources::mafResource);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Resource);
        mafEventBusManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafResource allocation test case.
    void mafResourceAllocationTest();
    /// Test the add input functionality
    void mafResourceAddInputTest();
    /// Test the remove input functionality
    void mafResourceRemoveInputTest();
    /// Test the createMemento functionality.
    void mafResourceCreateMementoTest();

private:
    mafResource *m_Resource; ///< Test var.
};

void mafResourceTest::mafResourceAllocationTest() {
    QVERIFY(m_Resource != NULL);
}

void mafResourceTest::mafResourceAddInputTest() {
    mafResource *obj1 = mafNEW(mafResources::mafResource);
    obj1->setObjectName("Obj1");

    //! <snippet>
    mafResource *obj2 = mafNEW(mafResources::mafResource);
    obj2->setObjectName("Obj2");

    int idx = m_Resource->addInput(obj1);
    //! </snippet>
    QVERIFY(idx == 0);

    idx = m_Resource->setInput(obj2, 0);
    QVERIFY(idx == 0);
    mafResourceList *il =  m_Resource->inputList();
    int num_elem = il->count();
    QVERIFY(num_elem == 1);
    QVERIFY(obj2->isEqual(il->at(0)));

    idx = m_Resource->addInput(obj1);
    QVERIFY(idx == 1);

    idx = m_Resource->setInput(obj1, 0);
    QVERIFY(idx == 1);
    QVERIFY(obj2->isEqual(il->at(0)));

    mafDEL(obj1);
    mafDEL(obj2);
}

void mafResourceTest::mafResourceRemoveInputTest() {
    mafResource *obj1 = mafNEW(mafResources::mafResource);
    obj1->setObjectName("Obj1");

    mafResource *obj2 = mafNEW(mafResources::mafResource);
    obj2->setObjectName("Obj2");

    m_Resource->addInput(obj1);
    mafResourceList *il =  m_Resource->inputList();
    //! <snippet>
    m_Resource->removeInput(0);
    //! </snippet>
    int num_elem = il->count();

    QVERIFY(num_elem == 0);

    m_Resource->addInput(obj2);

    // try to remove out of range index.
    m_Resource->removeInput(5);
    num_elem = il->count();
    QVERIFY(num_elem == 1);

    // try to remove object not present.
    m_Resource->removeInput(obj1);
    num_elem = il->count();
    QVERIFY(num_elem == 1);

    mafDEL(obj1);
    mafDEL(obj2);
}

void mafResourceTest::mafResourceCreateMementoTest() {
    mafResource *obj1 = mafNEW(mafResources::mafResource);
    obj1->setObjectName("Obj1");

    m_Resource->addInput(obj1);
    m_Resource->setObjectName("Test Name");

    mafMemento *m = m_Resource->createMemento();

    QVERIFY(m != NULL);

    mafResource *myRes = mafNEW(mafResources::mafResource);
    // Assign it to the new one object so they become equals.
    // (the 'deep_memento flag at true to be copied also the hash value)
    myRes->setMemento(m, true);

    QVERIFY(myRes->isEqual(m_Resource));

    mafDEL(myRes);
    mafDEL(m);
    mafDEL(obj1);
}

MAF_REGISTER_TEST(mafResourceTest);
#include "mafResourceTest.moc"
