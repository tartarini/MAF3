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
#include <mafPipeData.h>
#include <mafProxy.h>
#include <mafInteractor.h>
#include <mafMemento.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

//------------------------------------------------------------------------------------------
/**
 Class name: testVMEPipeDataCustom
 This class implements the data pipe to be tested.
 */
class testVMEPipeDataCustom : public  mafPipeData {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipeData);

public:
    /// Object constructor.
    testVMEPipeDataCustom(const QString code_location = "");

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

testVMEPipeDataCustom::testVMEPipeDataCustom(const QString code_location) : mafPipeData(code_location), m_PipeLine("") {
}

void testVMEPipeDataCustom::createPipe() {
    m_PipeLine = "Created";
}

void testVMEPipeDataCustom::updatePipe(double t) {
    Superclass::updatePipe();
    m_PipeLine = "Updated";
    m_PipeLine.append(QString::number(t));
}
//------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------

class testVMEConcurrentAccess : public QObject {
    Q_OBJECT
    
public:
    /// Constructor
    testVMEConcurrentAccess(mafVME *vme);
    
signals:
    /// Finish the threaded elaboration.
    void finished();
    
public slots:
    /// Start the VME access.
    void startElaboration();
    
private:
    mafVME *m_VME; ///< input vme.
};

testVMEConcurrentAccess::testVMEConcurrentAccess(mafVME *vme) : QObject(NULL), m_VME(vme) {
    
}

void testVMEConcurrentAccess::startElaboration() {
    mafPipeData *pipe = m_VME->dataPipe();
    if (pipe == NULL) {
        m_VME->setDataPipe("testVMEPipeDataCustom");
    }
    
    QTime dieTime = QTime::currentTime().addSecs(1);
    while(QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }

    QVariantList b;
    int n = 0;
    while (n < 100) {
        for (int i = n; i < n+6; ++i) {
            b.append(32.5 * i);
        }
        b.clear();
        ++n;
    }
    
    QTime dieTime2 = QTime::currentTime().addSecs(1);
    while(QTime::currentTime() < dieTime2) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }

    m_VME->setDataPipe(NULL);

    QTime dieTime3 = QTime::currentTime().addSecs(1);
    while(QTime::currentTime() < dieTime3) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
    }

    mafMemento *m = m_VME->createMemento();
    if (m == NULL) {
        qDebug() << mafTr("Problem creating VME memento");
    }
    mafDEL(m);
    
    emit finished();
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
        mafRegisterObject(testVMEPipeDataCustom);
        //! <snippet>
        m_VME = mafNEW(mafResources::mafVME);
        //! </snippet>
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_VME);
        mafUnregisterObject(testVMEPipeDataCustom);
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
    
    /// Test the concurrent access to the VME.
    void mafVMEConcurrentAccessTest();

private:
    mafVME *m_VME; ///< Test var.
};

void mafVMETest::mafVMEAllocationTest() {
    QVERIFY(m_VME != NULL);
    mafDataSetCollection *collection = m_VME->dataSetCollection();
    QVERIFY(collection != NULL);
}

void mafVMETest::mafVMEDataPipeTest() {
    mafPipeData *dp = m_VME->dataPipe();
    QVERIFY(dp == NULL);

    m_VME->setDataPipe("testVMEPipeDataCustom");
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
    mafDEL(i);
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

void mafVMETest::mafVMEConcurrentAccessTest() {
    QVariantList mainBounds;
    mainBounds << 0.0;
    mainBounds << 100.0;
    mainBounds << 0.0;
    mainBounds << 200.0;
    mainBounds << -20.0;
    mainBounds << 80.0;
    
    QThread thread;
    testVMEConcurrentAccess *accessor = new testVMEConcurrentAccess(m_VME);
    accessor->moveToThread(&thread);
    connect(&thread, SIGNAL(started()), accessor, SLOT(startElaboration()));
    connect(accessor, SIGNAL(finished()), &thread, SLOT(quit()), Qt::DirectConnection);
    
    thread.start();
    qDebug() << mafTr("Thread started...");
    
    mafMemento *memento = m_VME->createMemento();
    
    while (thread.isRunning()) {
        ;
    }
    
    mafDEL(memento);
    delete accessor;
    
    qDebug() << mafTr("**** End of VME concurrent access ****");
}

MAF_REGISTER_TEST(mafVMETest);
#include "mafVMETest.moc"
