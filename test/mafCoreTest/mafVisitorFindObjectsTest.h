/*
 *  mafVisitorFindObjectsTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafVisitorFindObjects.h>

class testVisitorFindObjectsCustom;

/**
 Class name: mafVisitorFindObjectsTest
 This class implements the test suite for mafVisitorFindObjects.
 */
class mafVisitorFindObjectsTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafVisitorFindObjects allocation test case.
    void mafVisitorFindObjectsAllocationTest();
    /// Test the creation and update methods..
    void mafVisitorFindObjectsObjectListTest();

private:
    testVisitorFindObjectsCustom *m_VisitorFindObjects; ///< Test var.
};
