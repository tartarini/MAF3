/*
 *  mafMementoDataSetCollectionTest.cpp
 *  mafResourcesTest
 *
 *  Created by Roberto Mucci - Daniele Giunchi on 24/05/10.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

#if defined(_WIN32) || defined(WIN32)
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

void mafMementoDataSetCollectionTest::initTestCase() {
  // Create before the instance of the Serialization manager, which will register signals.
  bool res(false);
  res = mafInitializeModule(SERIALIZATION_LIBRARY_NAME);
  QVERIFY(res);
  mafMessageHandler::instance()->installMessageHandler();
}

void mafMementoDataSetCollectionTest::cleanupTestCase() {
    mafMessageHandler::instance()->shutdown();
}

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
    mafMatrix4x4 *matrix = new mafMatrix4x4();
    matrix->setToIdentity();
    (*matrix)(0,0) = 3;
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

#include "mafMementoDataSetCollectionTest.moc"
