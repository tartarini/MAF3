/*
 *  mafDataSetTest.cpp
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
#include <mafDataSet.h>
#include <mafContainer.h>
#include <mafEventBusManager.h>
#include <mafDataBoundaryAlgorithm.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

//------------------------------------------------------------------------------------------

/**
 Class name: testExternalDataType
 This class implements the external data type coming from an external library.
 */
class testExternalDataType {
public:
    /// Object constructor.
    testExternalDataType(int v) : m_Value(v) {}

    /// set the new value for the class.
    void setValue(int v) {m_Value = v;}
    /// Return the inner value.
    int value() {return m_Value;}

private:
    int m_Value; ///< Test variable for external data
};

/**
 Class name: testExternalBoundary
 This class implements the external data boundary.
 */
class testExternalBoundary {
public:
    int m_Range[2]; ///< Boundary var.
};

class testDataBoundaryCustom : public mafDataBoundaryAlgorithm {
public:
    /// Object constructor.
    testDataBoundaryCustom(const mafString code_location = "");

    /// Object destructor.
    ~testDataBoundaryCustom();

    /// Boundary algorithm.
    /*virtual*/ mafContainerInterface *calculateBoundary(mafContainerInterface *data, mafPoseMatrix *m);

    /// Boundary algorithm.
    /*virtual*/ mafCore::mafContainerInterface *calculateBoundary(double bounds[6], mafPoseMatrix *matrix = NULL);

    /// Return bounds of the bounding box.
    /*virtual*/ void bounds(double bounds[6]);

private:
    mafContainer<testExternalBoundary> m_Boundary; ///< boundary data.
};

testDataBoundaryCustom::testDataBoundaryCustom(const mafString code_location) : mafDataBoundaryAlgorithm(code_location) {
    m_Boundary = new testExternalBoundary();
    m_Boundary->m_Range[0] = m_Boundary->m_Range[1] = 0;
}

testDataBoundaryCustom::~testDataBoundaryCustom() {
}

mafContainerInterface *testDataBoundaryCustom::calculateBoundary(mafContainerInterface *data, mafPoseMatrix *m) {
    Q_UNUSED(m);
    mafContainer<testExternalDataType> *container = mafContainerPointerTypeCast(testExternalDataType, data);
    testExternalDataType *value = container->externalData();
    if(value != NULL) {
        int v = value->value();
        if(v < m_Boundary->m_Range[0]) m_Boundary->m_Range[0] = v;
        if(v > m_Boundary->m_Range[1]) m_Boundary->m_Range[1] = v;
    }
    return &m_Boundary;
}

void testDataBoundaryCustom::bounds(double bounds[6]) {
    Q_UNUSED(bounds);
}

mafCore::mafContainerInterface *testDataBoundaryCustom::calculateBoundary(double bounds[6], mafPoseMatrix *matrix) {
    Q_UNUSED(bounds);
    Q_UNUSED(matrix);
    return NULL;
}

//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
/**
 Class name: testDataSetObserver
 This class implements the observer object for mafDataSet.
 It track back all the changes made into the mafDataSet.
 */
class testDataSetObserver : public mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObject);

public:
    /// Object constructor.
    testDataSetObserver(const mafString code_location = "");

public slots:
    void dataDidConnected();
    void dataDidDisconnected();
};

testDataSetObserver::testDataSetObserver(QString code_location) : mafObjectBase(code_location) {
}

void testDataSetObserver::dataDidConnected() {
    mafMsgDebug("%s", mafTr("Dataset I'm observing has been connected!!").toAscii().data());
}

void testDataSetObserver::dataDidDisconnected() {
    mafMsgDebug("%s", mafTr("Dataset I'm observing has been disconnected!!").toAscii().data());
}

//------------------------------------------------------------------------------------------

/**
 Class name: mafDataSetTest
 This class implements the test suite for mafDataSet.
 */

//! <title>
//mafDataSet
//! </title>
//! <description>
//mafDataSet is the base resource class for MAF3.
//It provides basic API to set input and get output from a mafResource.
//! </description>
class mafDataSetTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafEventBusManager::instance();

        //! <snippet>
        m_DataSet = mafNEW(mafResources::mafDataSet);
        //! </snippet>

        // instantiate the observer and connect it to the event bus.
        m_Observer = mafNEW(testDataSetObserver);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        // Free allocated memory
        mafDEL(m_DataSet);
        mafDEL(m_Observer);
        // Shutdown eventbus singleton and core singletons.
        mafEventBusManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafDataSet allocation test case.
    void mafDataSetAllocationTest();
    /// Test the external data value assignment.
    void mafDataSetValueTest();
    /// Boundary algorithm test.
    void mafDataSetBoundaryTest();

private:
    mafDataSet *m_DataSet; ///< Test var.
    testDataSetObserver *m_Observer; ///< Test observer
};

void mafDataSetTest::mafDataSetAllocationTest() {
    QVERIFY(m_DataSet != NULL);
}

void mafDataSetTest::mafDataSetValueTest() {
    // Create the container with the external data type.
    mafContainer<testExternalDataType> container;
    container = new testExternalDataType(3);

    // ... and assign it to the mafDataSet; should start the connected event.
    m_DataSet->setDataValue(&container);

    // ... and now disconnect external data from mafDataSet
    // disconnect event should be raised.
    m_DataSet->setDataValue(NULL);

    testExternalDataType *external_data = container;
    delete external_data;
}

void mafDataSetTest::mafDataSetBoundaryTest() {
    // Create the boundary class and assign it to the mafDataSet.
    // The mafDataSet will manage the deletation of the algorithm when a new algorithm
    // will be assigned or when the mafDataSet itself will die.
    testDataBoundaryCustom *boundaryAlgorithm = mafNEW(testDataBoundaryCustom);
    m_DataSet->setBoundaryAlgorithm(boundaryAlgorithm);

    // Create the custom data and asign it to the mafDataSet.
    mafContainer<testExternalDataType> container;
    container = new testExternalDataType(3);
    m_DataSet->setDataValue(&container);

    // Try to setDataValue again, nothing should happen..
    m_DataSet->setDataValue(&container);

    // Retrieve the boundary (should be 0 - 3).
    mafContainerInterface *boundary_container = m_DataSet->dataBoundary();
    mafContainer<testExternalBoundary> *boundary = mafContainerPointerTypeCast(testExternalBoundary, boundary_container);
    int r0 = (*boundary)->m_Range[0];
    int r1 = (*boundary)->m_Range[1];
    QVERIFY(r0 == 0);
    QVERIFY(r1 == 3);

    testExternalDataType *external_data = container;
    delete external_data;
}

MAF_REGISTER_TEST(mafDataSetTest);
#include "mafDataSetTest.moc"
