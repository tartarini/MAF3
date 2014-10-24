/*
 *  mafMementoVMETest.h
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
#include "mafPipeData.h"
#include <mafProxy.h>
#include <mafExternalDataCodec.h>
#include <mafProxy.h>
#include <mafProxyInterface.h>
#include <mafVME.h>

//forward declarations;
class testExtDataType;
class testExtDataCodecCustom;
class testDataPipe;

/**
 Class name: mafMementoVMETest
 This class implements the test suite for mafMementoVME.
 */
class mafMementoVMETest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafMementoVME allocation test case.
    void mafMementoVMEDefaultAllocationTest();
    /// mafMementoVME allocation test case.
    void mafMementoVMECustomAllocationTest();

private:
    mafResources::mafVME *m_VME; ///< Test var.
    mafResources::mafDataSet *m_DataSet; 
};
