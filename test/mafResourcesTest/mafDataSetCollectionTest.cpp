/*
 *  mafDataSetCollectionTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafResourcesRegistration.h>
#include <mafProxy.h>
#include <mafMatrix.h>
#include <mafDataSetCollection.h>
#include <mafDataSet.h>

using namespace mafCore;
using namespace mafResources;

/**
 Class name: testPoseObserver
 observer class that is used to receive modifiedObject notifications
 */
class testPoseObserver : public QObject {
    Q_OBJECT

public Q_SLOTS:
    /// Set at true the value of modified flag status.
    void turnOnModifyFlag();

public:
    /// Object constructor.
    testPoseObserver();

    /// Return the modified flag status.
    bool isModified();

    /// Reset the modify flag status to false.
    void resetModifyFlag();

private:
    bool m_Modified; ///< Modify flag.
};

testPoseObserver::testPoseObserver() : QObject(), m_Modified(false) {
}

inline bool testPoseObserver::isModified() {
    return m_Modified;
}

void testPoseObserver::turnOnModifyFlag() {
    m_Modified = true;
}

void testPoseObserver::resetModifyFlag() {
    m_Modified = false;
}

//--------------------------------------------------------------------------------------------------------
/**
 Class name: testCustomExternalData
 This class implements the external data type to give as value for mafVME after has been wrapped with mafProxy.
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
 This class implements the external data type to give as value for mafVME after has been wrapped with mafProxy.
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

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        //! <snippet>
        m_Collection = mafNEW(mafResources::mafDataSetCollection);
        //! </snippet>

        m_PoseObserver = new testPoseObserver;
        connect(m_Collection, SIGNAL(modifiedObject()), m_PoseObserver, SLOT(turnOnModifyFlag()), Qt::DirectConnection);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Collection);
        delete m_PoseObserver;
        mafMessageHandler::instance()->shutdown();
    }

    /// mafDataSetCollection allocation test case.
    void collectionAllocationTest();
    /// Test orientation and pose for mafDataSetCollection
    void collectionPoseMatrixTest();
    /// Test the pose matrix synchronization
    void collectionSynchronizePoseTest();
    /// Test new item insertion
    void collectionInsertItemTest();
    /// Test the setDataSet method
    void collectionDataSetTest();
    /// Test the remove item from the collection
    void collectionRemoveItemTest();

private:
    mafDataSetCollection *m_Collection; ///< Test var.
    testPoseObserver *m_PoseObserver; ///< Modify dataset collection's pose observer;
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

    QVERIFY(mafEquals(pose[0], rx, 1e-7));
    QVERIFY(mafEquals(pose[1], ry, 1e-7));
    QVERIFY(mafEquals(pose[2], rz, 1e-7));
}

void mafDataSetCollectionTest::collectionSynchronizePoseTest() {
    // reset the modify flag
    m_PoseObserver->resetModifyFlag();

    mafMatrix m;
    m.setIdentity();
    m.setElement(0, 1, 2.5);

    // Assign a pose matrix (collection notify the changes)
    m_Collection->setPose(m);
    bool res = m_PoseObserver->isModified();
    QVERIFY(res);

    // reset again the modify flag
    m_PoseObserver->resetModifyFlag();

    m.setIdentity();
    m_Collection->synchronizeItemWithPose(m);
    
    res = m_PoseObserver->isModified();
    QVERIFY(res);
}

void mafDataSetCollectionTest::collectionInsertItemTest() {
    // Create a test matrix to add to the collection.
    mafMatrix *newMatrix = new mafMatrix();
    newMatrix->setIdentity();
    
    newMatrix->setElement(0, 3, 5.0);
    newMatrix->setElement(1, 3, 1.3);
    newMatrix->setElement(2, 3, 4.1);

    mafDataSet *item = mafNEW(mafResources::mafDataSet);
    item->setPoseMatrix(newMatrix);

    // Insert a new pose matrix at the given timestamp.
    //! <snippet>
    bool result_insert = m_Collection->insertItem(item, 1.5);
    //! </snippet>
    cv::Mat mat;
    
    QVERIFY(result_insert);

    //! <snippet>
    mafMatrix *m = m_Collection->poseMatrix(1.5);
    //! </snippet>
    
    QVERIFY(m->element(1,2) == newMatrix->element(1,2));
    
    mafDEL(item);

    delete newMatrix;
}

void mafDataSetCollectionTest::collectionDataSetTest() {
    // Create the container for the external data type.
    mafProxy<testCustomExternalData> container;
    container = new testCustomExternalData();

    // Create the dataset that will contain the external data value.
    mafDataSet *data = mafNEW(mafResources::mafDataSet);
    data->setDataValue(&container);

    bool result = false;

    // initialize the VME with this first mafDataSet.
    // The mafVME has no data value inserted before, so it should accept it and the type of the mafVME
    // should became compliant with this external data type.
    result = m_Collection->setDataSet(data);

    QVERIFY(result);

    // Create the container for another external data type (different from before).
    mafProxy<testCustomAnotherExternalData> another_container;
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
    mafMatrix *newMatrix = new mafMatrix();
    newMatrix->setIdentity();
    
    newMatrix->setElement(0, 3, 5.0);
    newMatrix->setElement(1, 3, 1.3);
    newMatrix->setElement(2, 3, 4.1);

    mafDataSet *item = mafNEW(mafResources::mafDataSet);
    item->setPoseMatrix(newMatrix);

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
    
    delete newMatrix;
}

MAF_REGISTER_TEST(mafDataSetCollectionTest);
#include "mafDataSetCollectionTest.moc"
