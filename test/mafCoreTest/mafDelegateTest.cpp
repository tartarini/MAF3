/*
 *  mafDelegateTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 23/11/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

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


void mafDelegateTest::initTestCase() {
    m_Delegate = new testDelegate;
}

/// Cleanup test variables memory allocation.
void mafDelegateTest::cleanupTestCase() {
    delete m_Delegate;
}

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

#include "mafDelegateTest.moc"
