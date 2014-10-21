/*
 *  mafInteractorTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 07/10/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

using namespace mafCore;
using namespace mafResources;

void mafInteractorTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    mafResourcesRegistration::registerResourcesObjects();
    m_Interactor = mafNEW(mafInteractor);
}

void mafInteractorTest::cleanupTestCase() {
    mafDEL(m_Interactor);
    mafMessageHandler::instance()->shutdown();
}

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

#include "mafInteractorTest.moc"
