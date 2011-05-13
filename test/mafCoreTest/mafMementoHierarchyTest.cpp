/*
 *  mafMementoHierarchyTest.cpp
 *  mafCoreTest
 *
 *  Created by Roberto Mucci on 24/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafProxy.h>
#include <mafExternalDataCodec.h>
#include <mafProxy.h>
#include <mafProxyInterface.h>

using namespace mafCore;

//! <title>
//mafMementoHierarchy
//! </title>
//! <description>
//mafMementoHierarchy aims to store the tree structure in which each item
//represent a data with its own memento.
//! </description>

//------------------------------------------------------------------------------------------

/**
 Class name: testExtDataType
 This class implements the external data type coming from an external library.
 */
class testExtDataType {
public:
    /// Object constructor.
    testExtDataType(QString v) : m_Value(v) {}

    /// set the new value for the class.
    void setValue(QString v) {m_Value = v;}
    /// Return the inner value.
    QString value() {return m_Value;}

private:
    QString m_Value; ///< Test variable for external data
};



//------------------------------------------------------------------------------------------

/**
 Class name: mafMementoHierarchyTest
 This class implements the test suite for mafMementoHierarchy.
 */
class mafMementoHierarchyTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        // Create before the instance of the Serialization manager, which will register signals.

        mafMessageHandler::instance()->installMessageHandler();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafMessageHandler::instance()->shutdown();
    }

    /// mafMementoHierarchy allocation test case.
    void mafMementoHierarchyDefaultAllocationTest();
    /// mafMementoHierarchy allocation test case.
    void mafMementoHierarchyCustomAllocationTest();

private:
};

void mafMementoHierarchyTest::mafMementoHierarchyDefaultAllocationTest() {
}

void mafMementoHierarchyTest::mafMementoHierarchyCustomAllocationTest() {
}

MAF_REGISTER_TEST(mafMementoHierarchyTest);
#include "mafMementoHierarchyTest.moc"
