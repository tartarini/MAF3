/*
 *  mafObjectFactoryTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafObjectBase.h>
#include <mafObject.h>
#include <mafSmartPointer.h>

using namespace mafCore;

/**
 Class name: mafObjectFactoryTest
 This class implements the test suite for mafObjectFactory.
 */

//! <title>
//mafObjectFactory
//! </title>
//! <description>
//mafObjectFactory represent the factory for all the MAF3 objects.
//Objects are instantiated by calling the static method CreateObject and passing as argument the typename of the object to create.
//! </description>

class mafObjectFactoryTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// register new object in factory test case.
    void registerObjectTest();

    /// create object instance test case.
    void instantiateObjectTest();

    /// create object instance test case (return the base type).
    void instantiateObjectBaseTest();

    /// create object instance test case.
    void instantiateQObjectTest();

    /// create object instance test case (return the base type).
    void instantiateQObjectFromStringTest();

    /// unregister object test case.
    void unregisterObjectTest();

    /// smart pointer creation test case.
    void instantiateSmartObjectTest();

    /// smart pointer creation test case.
    void bindObjectToIconTest();

private:
};
