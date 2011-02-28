/*
 *  mafVMETest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafResourcesRegistration.h>
#include <mafDataPipe.h>
#include <mafContainer.h>
#include <mafInteractor.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

//------------------------------------------------------------------------------------------
/**
 Class name: testVMEDataPipeCustom
 This class implements the data pipe to be tested.
 */
class testVMEDataPipeCustom : public  mafDataPipe {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafDataPipe);

public:
    /// Object constructor.
    testVMEDataPipeCustom(const QString code_location = "");

    /// Initialize and create the pipeline
    /*virtual*/ void createPipe();

    /// Return the string variable initializated and updated from the data pipe.
    QString pipeline() {return m_PipeLine;}

public slots:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);

private:
    QString m_PipeLine; ///< Test Var.
};

testVMEDataPipeCustom::testVMEDataPipeCustom(const QString code_location) : mafDataPipe(code_location), m_PipeLine("") {
}

void testVMEDataPipeCustom::createPipe() {
    m_PipeLine = "Created";
}

void testVMEDataPipeCustom::updatePipe(double t) {
    Superclass::updatePipe();
    m_PipeLine = "Updated";
    m_PipeLine.append(QString::number(t));
}
//------------------------------------------------------------------------------------------


/**
 Class name: mafVMETest
 This class implements the test suite for mafVME.
 */

 //! <title>
//mafVME
//! </title>
//! <description>
//mafVME defines the base class for the VMEs in MAF3.
//! </description>

class mafVMETest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObject(testVMEDataPipeCustom);
        //! <snippet>
        m_VME = mafNEW(mafResources::mafVME);
        //! </snippet>
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_VME);
        mafMessageHandler::instance()->shutdown();
    }

    /// mafVME allocation test case.
    void mafVMEAllocationTest();

    /// Data pipe test suite
    void mafVMEDataPipeTest();

    /// Memento test suite.
    void mafVMEMementeTest();

    /// Interactor assignment test suite.
    void mafVMEInteractorTest();

    /// Output data test suite.
    void mafVMEOutputDataTest();

private:
    mafVME *m_VME; ///< Test var.
};

void mafVMETest::mafVMEAllocationTest() {
    QVERIFY(m_VME != NULL);
    mafDataSetCollection *collection = m_VME->dataSetCollection();
    QVERIFY(collection != NULL);
}

void mafVMETest::mafVMEDataPipeTest() {
    mafDataPipe *dp = m_VME->dataPipe();
    QVERIFY(dp == NULL);

    m_VME->setDataPipe("testVMEDataPipeCustom");
    dp = m_VME->dataPipe();
    QVERIFY(dp != NULL);
}

void mafVMETest::mafVMEMementeTest() {
    // Get a snapshot of the VME (its name is the empty string
    mafMemento *m = m_VME->createMemento();

    // Assign a name to the VME.
    m_VME->setObjectName("Test VME");

    // Assign the previous memento and check the name again.
    // The name should be the empty string again.
    m_VME->setMemento(m);
    QString name;
    name = m_VME->objectName();
    QVERIFY(name.isEmpty());

    // Free the memory associated with the requested memento.
    mafDEL(m);
}

void mafVMETest::mafVMEInteractorTest() {
    mafInteractor *inter = m_VME->interactor();
    QVERIFY(inter == NULL);

    mafInteractor *i = mafNEW(mafInteractor);
    m_VME->setInteractor(i);

    inter = m_VME->interactor();
    QVERIFY(inter != NULL);

    // Remove the interactor (and delete it)
    m_VME->setInteractor(NULL);
}

void mafVMETest::mafVMEOutputDataTest() {
    // Ask the output data
    // datapipe exists => the output will be the output of the data pipe.
    // Default output for datapipe is the input VME itself.
    mafDataSet *output = m_VME->outputData();
    QVERIFY(output != NULL);

    // Delete the current data pipe.
    m_VME->setDataPipe(NULL);

    // This time the output data will be the
    // element of the dataset collection at the current timestamp.
    output = m_VME->outputData();
    QVERIFY(output != NULL);
    // This time an empty mafDataSet will be returned.
    QVERIFY(output->dataBoundary() == NULL);
    QVERIFY(output->dataValue() == NULL);
}

MAF_REGISTER_TEST(mafVMETest);
#include "mafVMETest.moc"
