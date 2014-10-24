/*
 *  mafVisitorFindObjectsByHashCode.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafObject.h>

#include <mafVisitorFindObjectsByHashCode.h>

//! <title>
//mafVisitorFindObjectsByHashCode
//! </title>
//! <description>
//mafVisitorFindObjectsByHashCode define a modality of visiting object checking the hash code
//giving in input from the constructor of the visitor
//! </description>

/**
 Class name: mafVisitorFindObjectsByHashCodeTest
 This class implements the test suite for mafObject.
 */
class mafVisitorFindObjectsByHashCodeTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// register new object in factory test case.
    void visitTest();

private:
    mafObject *m_ObjTestVar1; ///< Test variable
    mafObject *m_ObjTestVar2; ///< Test variable
};
