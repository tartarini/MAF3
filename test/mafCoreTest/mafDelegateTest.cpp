/*
 *  mafDelegateTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 23/11/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
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

    /// Return the value of test variable
    int myVariable();

public Q_SLOTS:
    /// Increase the value of internal variable.
    void myCustomMethod();

    /// Assign a value to the internal variable.
    void setMyVariable(int v);

private:
    int m_MyVariable; ///< Test var.
};

testDelegate::testDelegate() : mafCore::mafDelegate(), m_MyVariable(0) {
}

void testDelegate::myCustomMethod() {
    ++m_MyVariable;
}

int testDelegate::myVariable() {
    return m_MyVariable;
}

void testDelegate::setMyVariable(int v) {
    m_MyVariable = v;
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

    /// Test the method execution.
    void executeMethodTest();

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

void mafDelegateTest::executeMethodTest() {
    m_Delegate->executeMethod("badMethodSignature");
    // No execution => m_MyVariable is the same as before.
    int v = m_Delegate->myVariable();
    QVERIFY(v == 0);
    
    m_Delegate->executeMethod("myCustomMethod()");
    // Method executed correctly.
    v = m_Delegate->myVariable();
    QVERIFY(v == 1);

    int val = 3;
    mafArgumentList argList;
    argList.append(mafArgument(int, val));
    m_Delegate->executeMethod("setMyVariable(int)", &argList);

    v = m_Delegate->myVariable();
    QVERIFY(v == val);
}

MAF_REGISTER_TEST(mafDelegateTest);
#include "mafDelegateTest.moc"
