/*
 *  mafVisitorTest.h
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

#include <mafVisitor.h>

class testVisitorImplement;

/**
 Class name: mafVisitorTest
 This class implements the test suite for mafVisitor.
 */
class mafVisitorTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// register new object in factory test case.
    void visitorTest();

private:
    testVisitorImplement *m_VisitorImplement; ///< Test variable
    mafObject *m_Object; ///< test var.
};
