/*
 *  mafMementoObjectTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafMementoObject.h>
#include <mafObject.h>

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

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// create new object and check that is not NULL test case.
    void mafMementoConstructorTest();

private:
};
