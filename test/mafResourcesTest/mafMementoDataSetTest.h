/*
 *  mafMementoDataSetTest.h
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafDataSet.h>
#include <mafProxy.h>
#include <mafExternalDataCodec.h>
#include <mafProxy.h>
#include <mafProxyInterface.h>

//forward declarations
class testExternalDataType;
class testExternalDataCodecCustom;

/**
 Class name: mafMementoDataSetTest
 This class implements the test suite for mafMementoDataSet.
 */
class mafMementoDataSetTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafMementoDataSet allocation test case.
    void mafMementoDataSetDefaultAllocationTest();
    /// mafMementoDataSet allocation test case.
    void mafMementoDataSetCustomAllocationTest();

private:
    mafResources::mafDataSet *m_DataSet; ///< Test var.
};
