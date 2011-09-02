/*
 *  mafProxyInterfaceTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreRegistration.h>
#include <mafProxyInterface.h>

using namespace mafCore;

/**
 Class name: mafProxyInterfaceTest
 This class implements the test suite for mafProxyInterface.
 */
class mafProxyInterfaceTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_ContainerInterface = new mafProxyInterface();
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        delete m_ContainerInterface;
    }

    /// Create new object and check that is not NULL test case.
    void mafProxyInterfaceAllocationTest();

    /// Test the set/get external data type.
    void externalDataTypeTest();

    /// Test the set/get external codec type.
    void externalCodecTypeTest();

private:
    mafProxyInterface *m_ContainerInterface; ///< Test var.
};

void mafProxyInterfaceTest::mafProxyInterfaceAllocationTest() {
    QVERIFY(m_ContainerInterface != NULL);
}

void mafProxyInterfaceTest::externalDataTypeTest() {
    QString ext("myCustomType");
    m_ContainerInterface->setExternalDataType(ext);
    QCOMPARE(m_ContainerInterface->externalDataType(), ext);
}

void mafProxyInterfaceTest::externalCodecTypeTest() {
    QString ext("myCustomCodecType");
    m_ContainerInterface->setExternalCodecType(ext);
    QCOMPARE(m_ContainerInterface->externalCodecType(), ext);
}

MAF_REGISTER_TEST(mafProxyInterfaceTest);
#include "mafProxyInterfaceTest.moc"

