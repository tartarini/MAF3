/*
 *  mafObjectRegistryTest.cpp
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafObject.h>
#include <mafVisitor.h>

using namespace mafCore;

//forward declaration
class testVisitorCounter;


/**
 Class name: mafObjectRegistryTest
 This class implements the test suite for mafObjectRegistry.
 */

//! <title>
//mafObjectRegistry
//! </title>
//! <description>
//mafObjectRegistry is a singleton which provides the place where allocated objects
//are monitored. It allows to monitor when, where and who allocated the object.
//
//When the constructor of a mafObjectBase is called, the object is added to the
//mafObjectRegistry instance. Similarly, when an object is deleted
//"mafObjectRegistry::instance()->removeObject(m_ObjectId)" is called and it will be
//removed from the registry. This class is useful for monitoring memory allocation,
//detect memory leaks and for garbage collection functionality.
//
//It also define the method "void dumpLiveObjects()" which dumps into default output
//stream all live objects still allocated and not deleted.
//! </description>

class mafObjectRegistryTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// create new object and check that is not NULL test case.
    void mafObjectRegistryConstructorTest();

    /// test the live objects presence.
    void liveObjectsTest();

    /// test the method for finding an object from its hash code
    void objectFromHashTest();

    /// Test the apply visitor method.
    void applyVisitorToObjectListThreadedTest();

private:
    mafObjectRegistry *m_Registry; ///< Test var.
};
