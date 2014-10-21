/*
 *  mafClassFactoryTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTestSuite.h"
#include <mafClassFactory.h>
#include <mafObject.h>

/**
 Class name: mafClassFactoryTest
 This class implements the test suite for mafClassFactory.
 */
class mafClassFactoryTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// create new object and check that is not NULL test case.
    void mafClassFactoryAllocationTest();

private:
    mafClassFactory<mafCore::mafObject> m_ClassFactory; ///< Test var.
};

