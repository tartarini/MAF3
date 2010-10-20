/*
 *  mafEventBridgeVTKTest.cpp
 *  mafPluginVTKTest
 *
 *  Created by Roberto Mucci on 26/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafEventBridgeVTK.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCommand.h>


//using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;

/**
Class name: testInteractionManagerCustom
This class defines the custom manager class used to test the mafInteraction manager interface API.
*/
class testInteractionManagerCustom : public mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    testInteractionManagerCustom(mafCore::mafObjectBase);

public:
    /// Object constructor
    testInteractionManagerCustom(const mafString code_location = "");

    int m_Counter; ///< Test variable;


public slots:
    /// observer needed to receive the 'maf.local.resources.interaction.leftButtonPress' signal
    void leftButtonPress();

    void mouseMove();

signals:
    /// left button pressed.
    void leftButtonPressSignal();

    void mouseMoveSignal();

};

testInteractionManagerCustom::testInteractionManagerCustom(QString code_location) : mafObjectBase(code_location) {
    mafRegisterLocalSignal("maf.local.resources.interaction.leftButtonPress", this, "leftButtonPressSignal()");
    mafRegisterLocalCallback("maf.local.resources.interaction.leftButtonPress", this, "leftButtonPress()");

    mafRegisterLocalSignal("maf.local.resources.interaction.mouseMove", this, "mouseMoveSignal()");
    mafRegisterLocalCallback("maf.local.resources.interaction.mouseMove", this, "mouseMove()");
    m_Counter = 0;

}

void testInteractionManagerCustom::leftButtonPress() {
    m_Counter++;
}

void testInteractionManagerCustom::mouseMove() {
    m_Counter++;
}


/**
 Class name: mafEventBridgeVTKTest
 This class implements the test suite for mafEventBridgeVTK.
 */

//! <title>
//mafEventBridgeVTK
//! </title>
//! <description>
//mafEventBridgeVTK crbetween VTk evengts and QT.
//! </description>

class mafEventBridgeVTKTest : public QObject {
    Q_OBJECT

private slots:

    /// Initialize test variables
    void initTestCase() {
        m_CustomManager = mafNEW(testInteractionManagerCustom);
        m_EventBridge = mafNEW(mafPluginVTK::mafEventBridgeVTK);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_EventBridge);
        mafDEL(m_CustomManager);
    }

    /// mafEventBridgeVTKTest allocation test case.
    void mafEventBridgeVTKAllocationTest();

    /// mafEventBridgeVTKTest event connection test case.
    void mafEventBridgeVTKConnectionTest();


private:
    mafEventBridgeVTK *m_EventBridge; ///< Test var.
    testInteractionManagerCustom *m_CustomManager; ///< Test var.
};

void mafEventBridgeVTKTest::mafEventBridgeVTKAllocationTest() {
    QVERIFY(m_EventBridge != NULL);
}

void mafEventBridgeVTKTest::mafEventBridgeVTKConnectionTest() {
    QVERIFY(m_EventBridge != NULL);

    vtkRenderWindow *renWin = vtkRenderWindow::New();
    vtkRenderer *renderer = vtkRenderer::New();
    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();

    renWin->AddRenderer(renderer);
    iren->SetRenderWindow(renWin);

    m_EventBridge->setInteractor(iren);
    //Send some interaction events by VTK
    iren->Initialize();
    iren->InvokeEvent(vtkCommand::LeftButtonPressEvent, NULL);
    iren->Enable();
    iren->InvokeEvent(vtkCommand::MouseMoveEvent, NULL);

    //Check if events has been captured by testInteractionManagerCustom
    QVERIFY(m_CustomManager->m_Counter == 1);
}

MAF_REGISTER_TEST(mafEventBridgeVTKTest);
#include "mafEventBridgeVTKTest.moc"
