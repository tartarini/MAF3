/*
 *  mafMementoObjectTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafMementoObject.h>
#include <mafObject.h>

using namespace mafCore;

//! <title>
//mafMementoObject
//! </title>
//! <description>
//mafMementoObject defines the MAF3 base object for storing a mafObject state implementing
//a sort of undo mechanism for the object's state. It is used to restore a previous stored
//object state.
//! </description>

/**
 Class name: mafMementoObjectTest
 This class implements the test suite for mafMementoObject.
 */
class mafMementoObjectTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
    }

    /// create new object and check that is not NULL test case.
    void mafMementoConstructorTest();

private:
};

void mafMementoObjectTest::mafMementoConstructorTest() {
    mafObject *obj = mafNEW(mafCore::mafObject);
    obj->setObjectName("Test memento");
    //! <snippet>
    mafMementoObject *memento = new mafMementoObject(obj, obj->tagList(), obj->dictionary(), mafCodeLocation);
    //! </snippet>
    QVERIFY(memento != NULL);
    mafDEL(memento);
    mafDEL(obj);
}

MAF_REGISTER_TEST(mafMementoObjectTest);
#include "mafMementoObjectTest.moc"

