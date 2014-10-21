/*
 *  mafMementoTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 27/03/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

using namespace mafCore;

void mafMementoTest::initTestCase() {
    m_Memento = mafNEW(mafCore::mafMemento);
}

void mafMementoTest::cleanupTestCase() {
    mafDEL(m_Memento);
}

void mafMementoTest::mafMementoConstructorTest() {
    QVERIFY(m_Memento != NULL);
}

void mafMementoTest::classTypeMementoTest() {
    QString ct("mafCustomType");
    m_Memento->setObjectClassType(ct);

    mafMementoPropertyItem item;
    item.m_Multiplicity = 1;
    item.m_Name = "name";
    item.m_Value = "myName";
    m_Memento->mementoPropertyList()->append(item);
    QCOMPARE(m_Memento->objectClassType(), ct);
}

void mafMementoTest::isEqualTest() {
    mafMemento *memento = mafNEW(mafCore::mafMemento);
    QString ct("differentClassType");
    memento->setObjectClassType(ct);
    bool res = m_Memento->isEqual(memento);
    QVERIFY(res == false);
    
    memento->setObjectClassType("mafCustomType");
    mafMementoPropertyItem item;
    item.m_Multiplicity = 1;
    item.m_Name = "name";
    item.m_Value = "myName";
    memento->mementoPropertyList()->append(item);
    res = m_Memento->isEqual(memento);
    QVERIFY(res == true);
    
    mafDEL(memento);
}

#include "mafMementoTest.moc"

