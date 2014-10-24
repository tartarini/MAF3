/*
 *  mafSerializationManagerTest.h
 *  mafSerializationTest
 *
 *  Created by Daniele Giunchi on 10/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include "mafSerializationManager.h"
#include <mafEventBusManager.h>
#include <mafCoreSingletons.h>

//forward declarations
class testCustomManager;

/**
 Class name: mafSerializationManagerTest
 This class implements the test suite for mafSerializationManager.
 */

//! <title>
//mafSerializationManager
//! </title>
//! <description>
// This singleton provides the facade class for the object serialization mechanism.
//! </description>

class mafSerializationManagerTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();
    
    /// Test library loading
    void mafSerializationLibraryLoading();
    /// mafSerializationManager allocation test case.
    void mafSerializationManagerAllocationTest();

    /// mafSerializationManager save test case.
    void mafSerializationManagerSaveTest();
    /// mafSerializationManager load test case.
    void mafSerializationManagerLoadTest();

private:
    mafSerialization::mafSerializationManager *m_SerializationManager; ///< Test var
    QString m_TestURL; ///< Test URL for file.
    testCustomManager *m_CustomManager; ///< Manager test var
};
