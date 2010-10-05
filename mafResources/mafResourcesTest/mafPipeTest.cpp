/*
 *  mafPipeTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafPipe.h>

using namespace mafCore;
using namespace mafResources;

//------------------------------------------------------------------------------------------
/**
 Class name: testPipeCustom
 This class implements the data pipe to be tested.
 */
class  testPipeCustom : public  mafPipe {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipe);

public:
    /// Object constructor.
    testPipeCustom(const mafString code_location = "");

    /// Initialize and create the pipeline
    /*virtual*/ void createPipe();

    /// Return the string variable initializated and updated from the data pipe.
    mafString pipeline() {return m_PipeLine;}

public slots:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);

private:
    mafString m_PipeLine; ///< Test Var.
};

testPipeCustom::testPipeCustom(const mafString code_location) : mafPipe(code_location), m_PipeLine("") {
}

void testPipeCustom::createPipe() {
    m_PipeLine = "Created";
}

void testPipeCustom::updatePipe(double t) {
    m_PipeLine = "Updated";
    m_PipeLine.append(mafString::number(t));
}
//------------------------------------------------------------------------------------------

/**
 Class name: mafPipeTest
 This class implements the test suite for mafPipe.
 */
class mafPipeTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafResourcesRegistration::registerResourcesObjects();
        m_Pipe = mafNEW(testPipeCustom);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Pipe);
    }

    /// mafPipe allocation test case.
    void mafPipeAllocationTest();
    /// Test the creation and update methods.
    void mafPipeCreationAndUpdateTest();
    /// Test the input management of the mafPipe
    void inputManagementTest();

private:
    testPipeCustom *m_Pipe; ///< Test var.
};

void mafPipeTest::mafPipeAllocationTest() {
    QVERIFY(m_Pipe != NULL);
}

void mafPipeTest::mafPipeCreationAndUpdateTest() {
    mafString res("Created");
    m_Pipe->createPipe();

    QCOMPARE(m_Pipe->pipeline(), res);

    res = "Updated1";
    m_Pipe->updatePipe(1);
    QCOMPARE(m_Pipe->pipeline(), res);
}

void mafPipeTest::inputManagementTest() {
    mafDataSet *data1 = mafNEW(mafResources::mafDataSet);
    mafDataSet *data2 = mafNEW(mafResources::mafDataSet);

    // try to set a 'bad' input number...
    int idx = m_Pipe->setInput(data1, 50);
    QVERIFY(idx == 0);

    int num = m_Pipe->inputList()->length();
    QVERIFY(num == 1);

    // add another input
    idx = m_Pipe->addInput(data2);
    QVERIFY(idx == 1);
    num = m_Pipe->inputList()->length();
    QVERIFY(num == 2);

    // remove the item at index 0
    m_Pipe->removeInput(data1);
    num = m_Pipe->inputList()->length();
    QVERIFY(num == 1);

    // Get the item at index 0
    // After that the old item at index 0 has been removed,
    // the item next to it should be moved at the index 0.
    mafDataSet *data = m_Pipe->inputList()->at(0);
    QCOMPARE(data, data2);

    // substitute the data2 with the data1 at index 0
    idx = m_Pipe->setInput(data1, 0);
    QVERIFY(idx == 0);

    // re-add the data1; will take the index 0 because it is already present.
    idx = m_Pipe->addInput(data1);
    QVERIFY(idx == 0);

    // remove all the items
    m_Pipe->removeInput(data1);
    num = m_Pipe->inputList()->length();
    QVERIFY(num == 0);

    // Create a collection
    mafDataSetCollection collection;
    collection.insertItem(data1, 0.0);
    collection.insertItem(data2, 1.0);
    idx = m_Pipe->addInputMulti(&collection);

    // The index returned is associated to the last data added.
    QVERIFY(idx == 1);

    // destroy the data => will be removed also from the input list of the pipe.
    mafDEL(data1);
    mafDEL(data2);

    // now the input list should be empty.
    num = m_Pipe->inputList()->length();
    QVERIFY(num == 0);
}

MAF_REGISTER_TEST(mafPipeTest);
#include "mafPipeTest.moc"
