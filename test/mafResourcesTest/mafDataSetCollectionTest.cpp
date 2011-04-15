/*
 *  mafDataSetCollectionTest.cpp
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
#include <mafContainer.h>

using namespace mafCore;
using namespace mafResources;

//--------------------------------------------------------------------------------------------------------
/**
 Class name: testCustomExternalData
 This class implements the external data type to give as value for mafVME after has beed wrapped with mafContainer.
 */
class testCustomExternalData {
public:
    /// Object constructor.
    testCustomExternalData() : m_Var(2) {}

    /// Return the value of the internal member variable.
    int var() {return m_Var;}

private:
    int m_Var; ///< Test var.
};

/**
 Class name: testCustomAnotherExternalData
 This class implements the external data type to give as value for mafVME after has beed wrapped with mafContainer.
 */
class testCustomAnotherExternalData {
public:
    /// Object constructor.
    testCustomAnotherExternalData() : m_Var(2) {}

    /// Return the value of the internal member variable.
    int var() {return m_Var;}

private:
    int m_Var; ///< Test var.
};

//--------------------------------------------------------------------------------------------------------



/**
 Class name: mafDataSetCollectionTest
 This class implements the test suite for mafDataSetCollection.
 */

 //! <title>
//mafDataSetCollection
//! </title>
//! <description>
//mafDataSetCollection is the class representing the time varying data for MAF3.
//! </description>
class mafDataSetCollectionTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        //! <snippet>
        m_Collection = mafNEW(mafResources::mafDataSetCollection);
        //! </snippet>
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Collection);
        mafMessageHandler::instance()->shutdown();
    }

    /// mafDataSetCollection allocation test case.
    void collectionAllocationTest();
    /// Test orientation and pose for mafDataSetCollection
    void collectionPoseMatrixTest();
    /// Test new item insertion
    void collectionInsertItemTest();
    /// Test the setDataSet method
    void collectionDataSetTest();
    /// Test the remove item from the collection
    void collectionRemoveItemTest();

private:
    mafDataSetCollection *m_Collection; ///< Test var.
};

void mafDataSetCollectionTest::collectionAllocationTest() {
    QVERIFY(m_Collection != NULL);
}

void mafDataSetCollectionTest::collectionPoseMatrixTest() {
    double x, y, z;
    double rx, ry, rz;

    x = 10.5;
    y = 2.35;
    z = 0.0;

    rx = 30.0;
    ry = 45.0;
    rz = 60.0;

    // First pose matrix; collection is empty, this matrix will be inserted
    // in this case at the current time: default = 0.0
    //! <snippet>
    m_Collection->setPose(rx, ry, rz, x, y, z);
    //! </snippet>

    double pose[3];
    m_Collection->orientations(pose);

    QCOMPARE(pose[0], rx);
    QCOMPARE(pose[1], ry);
    QCOMPARE(pose[2], rz);
}

void mafDataSetCollectionTest::collectionInsertItemTest() {
    // Create a test matrix to add to the collection.
    mafPoseMatrix *new_matrix = new mafPoseMatrix();
    new_matrix->set_identity();
    new_matrix->put(0,3,5.0);
    new_matrix->put(1,3,2.3);
    new_matrix->put(2,3,4.1);

    mafDataSet *item = mafNEW(mafResources::mafDataSet);
    item->setPoseMatrix(new_matrix);

    // Insert a new pose matrix at the given timestamp.
    //! <snippet>
    bool result_insert = m_Collection->insertItem(item, 1.5);
    //! </snippet>

    QVERIFY(result_insert);

    //! <snippet>
    mafPoseMatrix *m = m_Collection->poseMatrix(1.5);
    //! </snippet>
    QVERIFY(*m == *new_matrix);
    
    mafDEL(item);

    delete new_matrix;
    new_matrix = NULL;
}

void mafDataSetCollectionTest::collectionDataSetTest() {
    // Create the container for the external data type.
    mafContainer<testCustomExternalData> container;
    container = new testCustomExternalData();

    // Create the dataset that will contain the external data value.
    mafDataSet *data = mafNEW(mafResources::mafDataSet);
    data->setDataValue(&container);

    bool result = false;

    // initialize the VME with this first mafDataSet.
    // The mafVME has no data value inserted before, so it hsould accept it and the type of the mafVME
    // should became compliant with this external data type.
    result = m_Collection->setDataSet(data);

    QVERIFY(result);

    // Create the container for another external data type (different from before).
    mafContainer<testCustomAnotherExternalData> another_container;
    another_container = new testCustomAnotherExternalData();

    mafDataSet *another_data = mafNEW(mafResources::mafDataSet);
    another_data->setDataValue(&another_container);

    // Trying to set this new data type to the same VME should fail because the mafVME
    // has been initialized with a different data type.
    mafMessageHandler::instance()->testSuiteLogMode(true);
    result = m_Collection->setDataSet(another_data);
    mafMessageHandler::instance()->testSuiteLogMode(false);
    if(!result) {
        mafDEL(another_data);
    }
    QVERIFY(result == false);

    // Clean up allocated memory for external data type.
    testCustomExternalData *extData1 = container.externalData();
    delete extData1;
    testCustomAnotherExternalData *extData2 = another_container.externalData();
    delete extData2;
}

void mafDataSetCollectionTest::collectionRemoveItemTest() {
    // Create a test matrix to add to the collection.
    mafPoseMatrix *new_matrix = new mafPoseMatrix();
    new_matrix->set_identity();
    new_matrix->put(0,3,5.0);
    new_matrix->put(1,3,2.3);
    new_matrix->put(2,3,4.1);

    mafDataSet *item = mafNEW(mafResources::mafDataSet);
    item->setPoseMatrix(new_matrix);

    // Add a new item at a new timestamp.
    bool result_insert = m_Collection->insertItem(item, 3.5);
    item->release();
    QVERIFY(result_insert);

    // Test manual remove for the item but keep alive it so we can re-add later.
    bool result_remove = m_Collection->removeItem(item, true);
    QVERIFY(result_remove);

    // Add again the item
    result_insert = m_Collection->insertItem(item, 3.5);
    QVERIFY(result_insert);

    // Delete the item; it hsould be removed also from the collection.
    mafDEL(item);

    // Verify that there aren't items at timestamp 3.5
    const mafDataSetMap *data_map = m_Collection->collectionMap();
    mafDataSet *data = data_map->value(3.5, NULL);
    QVERIFY(data != NULL);
}

MAF_REGISTER_TEST(mafDataSetCollectionTest);
#include "mafDataSetCollectionTest.moc"
