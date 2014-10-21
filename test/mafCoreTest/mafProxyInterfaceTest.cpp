/*
 *  mafProxyInterfaceTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 27/03/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

using namespace mafCore;

void mafProxyInterfaceTest::initTestCase() {
    m_ContainerInterface = new mafProxyInterface();
}

void mafProxyInterfaceTest::cleanupTestCase() {
    delete m_ContainerInterface;
}

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

#include "mafProxyInterfaceTest.moc"

