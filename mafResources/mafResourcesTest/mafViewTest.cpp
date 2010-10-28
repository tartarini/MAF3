/*
 *  mafViewTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafResourcesRegistration.h>
#include <mafContainer.h>
#include <mafView.h>
#include <mafVME.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

/**
 Class name: mafViewTest
 This class implements the test suite for mafView.
 */

//! <title>
//mafView
//! </title>
//! <description>
//mafView defines the base class for the view in MAF3.
//! </description>

class mafViewTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        m_View = mafNEW(mafResources::mafView);
        m_View->create();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_View);
    }

    /// mafView allocation test case.
    void mafViewAllocationTest();

    /// mafView add & remove VME test case.
    void mafViewAddRemoveVMETest();

    /// mafView show VME test case.
    void mafViewShowVMETest();

    /// mafView show VME test case.
    void mafViewPlugVisualPipeTest();

private:
    mafView *m_View; ///< Test var.
};

void mafViewTest::mafViewAllocationTest() {
    QVERIFY(m_View != NULL);
}

void mafViewTest::mafViewAddRemoveVMETest() {
    mafVME *vme;
    vme = mafNEW(mafResources::mafVME);

    //add VME
    m_View->addVME(vme);

    //remove VME
    m_View->removeVME(vme);
    mafDEL(vme);
}

void mafViewTest::mafViewShowVMETest() {
    mafVME *vme;
    vme = mafNEW(mafResources::mafVME);

    //try to show a vme not added
    m_View->showVME(vme, true,"mafPipesLibrary::mafVisualPipeVTKSurface");

    // add vme and show (can not show, thos is a base class)
    m_View->addVME(vme);
    m_View->showVME(vme, false,"mafPipesLibrary::mafVisualPipeVTKSurface");
    mafDEL(vme);
}

void mafViewTest::mafViewPlugVisualPipeTest() {
    mafVME *vme;
    vme = mafNEW(mafResources::mafVME);

    m_View->plugVisualPipe("vtkPolyData","mafPipesLibrary::mafVisualPipeVTKSurface");
    mafDEL(vme);
}

MAF_REGISTER_TEST(mafViewTest);
#include "mafViewTest.moc"
