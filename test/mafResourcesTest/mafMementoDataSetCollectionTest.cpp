/*
 *  mafMementoDataSetCollectionTest.cpp
 *  mafResourcesTest
 *
 *  Created by Roberto Mucci on 24/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafDataSetCollection.h>
#include <mafProxy.h>
#include <mafExternalDataCodec.h>
#include <mafProxy.h>
#include <mafProxyInterface.h>

#ifdef WIN32
#define SERIALIZATION_LIBRARY_NAME "mafSerialization.dll"
#else
#ifdef __APPLE__
#define SERIALIZATION_LIBRARY_NAME "mafSerialization.dylib"
#else
#define SERIALIZATION_LIBRARY_NAME "mafSerialization.so"
#endif
#endif


using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

//! <title>
//mafMementoDataSetCollection
//! </title>
//! <description>
//mafMementoDataSetCollection aims to store a mafDataSetCollection state implementing a sort
//of undo mechanism for the object's state. This is used to restore
// a previous stored DataSet state (undo mechanism or serialization porpouses).
//! </description>


/**
 Class name: mafMementoDataSetCollectionTest
 This class implements the test suite for mafMementoDataSetCollection.
 */
class mafMementoDataSetCollectionTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
      // Create before the instance of the Serialization manager, which will register signals.
      bool res(false);
      res = mafInitializeModule(SERIALIZATION_LIBRARY_NAME);
      QVERIFY(res);
      mafMessageHandler::instance()->installMessageHandler();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafMessageHandler::instance()->shutdown();
    }

    /// mafMementoDataSetCollection allocation test case.
    void mafMementoDataSeCollectiontDefaultAllocationTest();
    /// mafMementoDataSetCollection test case.
    void mafMementoDataSetCollectionAllocationTest();

private:
    mafDataSet *m_DataSet; ///< Test var.
};

void mafMementoDataSetCollectionTest::mafMementoDataSeCollectiontDefaultAllocationTest() {
    //QVERIFY(m_DataSet != NULL);
    mafVME *vme = mafNEW(mafResources::mafVME);
    mafDataSetCollection *collection = vme->dataSetCollection();
    mafMemento *memento = collection->createMemento();
    QVERIFY(memento != NULL);
    mafDEL(vme);
    mafDEL(memento);
}

void mafMementoDataSetCollectionTest::mafMementoDataSetCollectionAllocationTest() {
    mafVME *vme = mafNEW(mafResources::mafVME);
    mafDataSetCollection *collection = vme->dataSetCollection();

    //Create dataset for the collection
    mafDataSet *firstData = mafNEW(mafResources::mafDataSet);
    mafDataSet *secondData = mafNEW(mafResources::mafDataSet);
    mafMatrix *matrix = new mafMatrix();
    matrix->setIdentity();
    matrix->setElement(0,0,3);
    firstData->setPoseMatrix(matrix);

    //Fill the mafDataSetCollection
    collection->insertItem(firstData, 0);
    collection->insertItem(secondData, 1);

    mafMemento *memento = collection->createMemento();
    mafVME *newVme = mafNEW(mafResources::mafVME);
    mafDataSetCollection *newCollection = newVme->dataSetCollection();
    newCollection->setMemento(memento);

    //Check if memento is equal to newCollection memento.
    mafMemento *returnMemento = newCollection->createMemento();
    QVERIFY(memento->isEqual(returnMemento));

    delete matrix;
    mafDEL(secondData);
    mafDEL(firstData);
    mafDEL(vme);
    mafDEL(newVme);
    mafDEL(memento);
    mafDEL(returnMemento);
}

MAF_REGISTER_TEST(mafMementoDataSetCollectionTest);
#include "mafMementoDataSetCollectionTest.moc"
