/*
 *  mafInteractorTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 07/10/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafInteractor.h>
#include <mafProxyInterface.h>

using namespace mafCore;
using namespace mafResources;

/**
 Class name: mafInteractorTest
 This class implements the test suite for mafInteractor.
 */
class mafInteractorTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        m_Interactor = mafNEW(mafInteractor);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Interactor);
        mafMessageHandler::instance()->shutdown();
    }

    /// mafInteractor allocation test case.
    void mafInteractorAllocationTest();

    /// mafInteractor second constructor allocation test case.
    void mafInteractorStyleConstructorAllocationTest();

    /// style assignment test suite.
    void styleAssignmentTest();

private:
    mafInteractor *m_Interactor; ///< Test var.
};

void mafInteractorTest::mafInteractorAllocationTest() {
    QVERIFY(m_Interactor);
    /*mafProxyInterfacePointer di = m_Interactor->defaultInteractionStyle();
    QVERIFY(di == NULL);
    mafProxyInterfacePointer ei = m_Interactor->editingInteractionStyle();
    QVERIFY(ei == NULL);*/
}

void mafInteractorTest::mafInteractorStyleConstructorAllocationTest() {
    // Create interactor styles to assign to the interactor.
    /*mafProxyInterface dis;
    mafProxyInterface eis;
    // Construct a new mafInteractor.
    mafInteractor *inter = new mafInteractor(&dis, &eis, mafCodeLocation);

    // Verify that the interactor styles are present.
    mafProxyInterfacePointer di = inter->defaultInteractionStyle();
    QVERIFY(di != NULL);
    mafProxyInterfacePointer ei = inter->editingInteractionStyle();
    QVERIFY(ei != NULL);

    mafDEL(inter);*/
}

void mafInteractorTest::styleAssignmentTest() {
    // Create interactor styles to assign to the interactor.
/*    mafProxyInterface dis;

    // Assign the style to the interactor...
    m_Interactor->setDefaultInteractionStyle(&dis);

    // Verify that the interactor styles are present.
    mafProxyInterfacePointer di = m_Interactor->defaultInteractionStyle();
    QVERIFY(di != NULL);*/
}

MAF_REGISTER_TEST(mafInteractorTest);
#include "mafInteractorTest.moc"
