/*
 *  mafMementoTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreRegistration.h>
#include <mafMemento.h>

/**
 Class name: mafMementoTest
 This class implements the test suite for mafMemento.
 */
class mafMementoTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// create new object and check that is not NULL test case.
    void mafMementoConstructorTest();
    /// Classtype test
    void classTypeMementoTest();

    /// isEqual Test
    void isEqualTest();

private:
    mafMemento *m_Memento;
};
