/*
 *  mafMementoTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreRegistration.h>
#include <mafMemento.h>

using namespace mafCore;

/**
 Class name: mafMementoTest
 This class implements the test suite for mafMemento.
 */
class mafMementoTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        m_Memento = mafNEW(mafCore::mafMemento);
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Memento);
    }

    /// create new object and check that is not NULL test case.
    void mafMementoConstructorTest();
    /// Classtype test
    void classTypeMementoTest();

    /// isEqual Test
    void isEqualTest();

private:
    mafMemento *m_Memento;
};

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

MAF_REGISTER_TEST(mafMementoTest);
#include "mafMementoTest.moc"

