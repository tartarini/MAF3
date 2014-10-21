/*
 *  mafProxyInterfaceTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreRegistration.h>
#include <mafProxyInterface.h>

/**
 Class name: mafProxyInterfaceTest
 This class implements the test suite for mafProxyInterface.
 */
class mafProxyInterfaceTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// Create new object and check that is not NULL test case.
    void mafProxyInterfaceAllocationTest();

    /// Test the set/get external data type.
    void externalDataTypeTest();

    /// Test the set/get external codec type.
    void externalCodecTypeTest();

private:
    mafProxyInterface *m_ContainerInterface; ///< Test var.
};
