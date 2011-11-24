/*
 *  mafDelegateTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 23/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreRegistration.h>
#include <mafDelegate.h>

//-------------------------------------------------------------------------
/**
 Class name: mafVisitorCustom
 Define a custom visitor for testing 'acceptVisitor' method.
 */
class testDelegate : public mafCore::mafDelegate {
    Q_OBJECT

public:
    testDelegate();

public Q_SLOTS:
    void myCustomMethod();
};

testDelegate::testDelegate() : mafCore::mafDelegate() {
}

void testDelegate::myCustomMethod() {
}
//-------------------------------------------------------------------------


using namespace mafCore;

/**
 Class name: mafDelegateTest
 This class implements the test suite for mafDelegate.
 */
class mafDelegateTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        m_Delegate = new testDelegate;
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        delete m_Delegate;
    }

    /// create new object and check that is not NULL test case.
    void mafDelegateConstructorTest();

    /// Test the presence of method definition
    void isMethodDefinedTest();

    /// Verify that the shouldExecuteLocalCode return true by default.
    void shouldExecuteLocalCodeTest();

private:
    testDelegate *m_Delegate; ///< Test variable.
};

void mafDelegateTest::mafDelegateConstructorTest() {
    QVERIFY(m_Delegate != NULL);
}

void mafDelegateTest::isMethodDefinedTest() {
    QString method("myCustomMethod()");
    // Existing method...
    bool res = m_Delegate->isMethodDefined(method);
    QVERIFY(res);

    // Not existing method...
    method = "badMethod(QString)";
    res = m_Delegate->isMethodDefined(method);
    QVERIFY(!res);
}

void mafDelegateTest::shouldExecuteLocalCodeTest() {
    QVERIFY(m_Delegate->shouldExecuteLocalCode());
}

MAF_REGISTER_TEST(mafDelegateTest);
#include "mafDelegateTest.moc"
