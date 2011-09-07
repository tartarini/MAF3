/*
 *  mafPipeDataTest.cpp
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
#include <mafPipeData.h>
#include <mafResourcesRegistration.h>
#include <mafVME.h>

using namespace mafCore;
using namespace mafResources;

//------------------------------------------------------------------------------------------
/**
 Class name: testDataPipeCustom
 This class implements the data pipe to be tested.
 */
class  testDataPipeCustom : public  mafPipeData {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipeData);

public:
    /// Object constructor.
    testDataPipeCustom(const QString code_location = "");

    /// Return the string variable initialized and updated from the data pipe.
    QString pipeline() {return m_PipeLine;}

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);

private:
    QString m_PipeLine; ///< Test Var.
};

testDataPipeCustom::testDataPipeCustom(const QString code_location) : mafPipeData(code_location), m_PipeLine("Created") {
}

void testDataPipeCustom::updatePipe(double t) {
    m_PipeLine = "Updated";
    m_PipeLine.append(QString::number(t));

    Superclass::updatePipe(t);
}

//------------------------------------------------------------------------------------------

/**
 Class name: mafPipeDataTest
 This class implements the test suite for mafPipeData.
 */
class mafPipeDataTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        m_DataPipe = mafNEW(testDataPipeCustom);
        m_Vme = mafNEW(mafResources::mafVME);
        m_Vme->setDataPipe(m_DataPipe);
        m_DataPipe->release();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Vme);
        mafMessageHandler::instance()->shutdown();
    }

    /// mafPipeData allocation test case.
    void mafPipeDataAllocationTest();
    /// Test the creation and update methods..
    void mafPipeDataCreationAndUpdateTest();

    /// Test the data pipe decoration mechanism.
    void decorateTest();

    /// Test add and remove input
    void addRemoveInputTest();

    /// Test add and remove input
    void outputTest();

private:
    testDataPipeCustom *m_DataPipe; ///< Test var.
    mafVME *m_Vme; ///< vme assigned to the data pipe
};

void mafPipeDataTest::mafPipeDataAllocationTest() {
    QVERIFY(m_DataPipe != NULL);
}

void mafPipeDataTest::mafPipeDataCreationAndUpdateTest() {
    QString res("Created");

    QString pipe = m_DataPipe->pipeline();
    QCOMPARE(pipe, res);

    res = "Updated1";
    m_DataPipe->setModified();
    m_DataPipe->output(1);
    pipe = m_DataPipe->pipeline();
    QCOMPARE(pipe, res);
}

void mafPipeDataTest::decorateTest() {
    // Instantiate and create the decorator data pipe.
    testDataPipeCustom *dpDecorator = mafNEW(testDataPipeCustom);
    QString res("Updated1");
    QString check;
    m_DataPipe->decorateWithDataPipe(dpDecorator);
    
    m_DataPipe->output(1);
    check = dpDecorator->pipeline();
    QCOMPARE(check, res);
    mafDEL(dpDecorator);
}

void mafPipeDataTest::addRemoveInputTest() {
    int num = 0;

    mafDataSet *data1 = mafNEW(mafResources::mafDataSet);
    mafDataSet *data2 = mafNEW(mafResources::mafDataSet);

    mafVME *vme1 = mafNEW(mafResources::mafVME);
    vme1->dataSetCollection()->insertItem(data1);

    mafVME *vme2 = mafNEW(mafResources::mafVME);
    vme2->dataSetCollection()->insertItem(data2);

    // Check if vme1 has been added.
    m_DataPipe->addInput(vme1);
    num = m_DataPipe->inputList()->length();
    QVERIFY(num == 2);

    // Check if vme2 has been added.
    m_DataPipe->addInput(vme2);
    num = m_DataPipe->inputList()->length();
    QVERIFY(num == 3);

    // Check if vme1 has been removed.
    m_DataPipe->removeInput(1);
    num = m_DataPipe->inputList()->length();
    QVERIFY(num == 2);

    // Get the vme2
    mafVME *vme = m_DataPipe->inputList()->at(1);
    QCOMPARE(vme, vme2);

    // Delete VMEs: they will removed from inputList
    mafDEL(data1);
    mafDEL(data2);
    mafDEL(vme1);
    mafDEL(vme2);
}

void mafPipeDataTest::outputTest() {
    // Instantiate and create the decorator data pipe.
    mafVME *vme1 = mafNEW(mafResources::mafVME);
    testDataPipeCustom *dp = mafNEW(testDataPipeCustom);
    vme1->setDataPipe(dp);
    dp->release();
    mafVME *out = dp->output();
    
    QVERIFY(out != NULL);
    mafDEL(vme1);
}

MAF_REGISTER_TEST(mafPipeDataTest);
#include "mafPipeDataTest.moc"
