/*
 *  mafContainerInterfaceTest.cpp
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
#include <mafContainerInterface.h>

using namespace mafCore;

/**
 Class name: mafContainerInterfaceTest
 This class implements the test suite for mafContainerInterface.
 */
class mafContainerInterfaceTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_ContainerInterface = new mafContainerInterface();
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        delete m_ContainerInterface;
    }

    /// Create new object and check that is not NULL test case.
    void mafContainerInterfaceAllocationTest();

    /// Test the set/get external data type.
    void externalDataTypeTest();

private:
    mafContainerInterface *m_ContainerInterface; ///< Test var.
};

void mafContainerInterfaceTest::mafContainerInterfaceAllocationTest() {
    QVERIFY(m_ContainerInterface != NULL);
}

void mafContainerInterfaceTest::externalDataTypeTest() {
    mafString ext("myCustomType");
    m_ContainerInterface->setExternalDataType(ext);
    QCOMPARE(m_ContainerInterface->externalDataType(), ext);
}


MAF_REGISTER_TEST(mafContainerInterfaceTest);
#include "mafContainerInterfaceTest.moc"

