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
#include <mafVisualPipeVTKSurface.h>

#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCommand.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>


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

    double *position();

    int m_Counter; ///< Test variable;
    double m_Pos[3]; ///< Test variable;


public slots:
    /// observer needed to receive the 'maf.local.resources.interaction.leftButtonPress' signal
    void leftButtonPress();

    /// observer needed to receive the 'maf.local.resources.interaction.vmePick' signal
    void pick(double *pos, mafCore::mafContainerInterface *interface);

signals:
    /// left button pressed.
    void leftButtonPressSignal();

    /// picked button pressed.
    void pickSignal(double *pos, mafCore::mafContainerInterface *interface);

};

testInteractionManagerCustom::testInteractionManagerCustom(QString code_location) : mafObjectBase(code_location) {
    mafIdProvider::instance()->createNewId("maf.local.resources.interaction.vmePick");
    mafRegisterLocalSignal("maf.local.resources.interaction.leftButtonPress", this, "leftButtonPressSignal()");
    mafRegisterLocalCallback("maf.local.resources.interaction.leftButtonPress", this, "leftButtonPress()");

    mafRegisterLocalSignal("maf.local.resources.interaction.vmePick", this, "pickSignal(double *,mafCore::mafContainerInterface *)");
    mafRegisterLocalCallback("maf.local.resources.interaction.vmePick", this, "pick(double *, mafCore::mafContainerInterface *)");
    m_Counter = 0;
}

void testInteractionManagerCustom::leftButtonPress() {
    m_Counter++;
}

void testInteractionManagerCustom::pick(double *pos, mafCore::mafContainerInterface *interface) {
     m_Counter++;
     m_Pos[0] = pos[0];
     m_Pos[1]= pos[1];
     m_Pos[2] = pos[2];
}
double *testInteractionManagerCustom::position() {
    return m_Pos;
}


/**
 Class name: mafEventBridgeVTKTest
 This class implements the test suite for mafEventBridgeVTK.
 */

//! <title>
//mafEventBridgeVTK
//! </title>
//! <description>
//mafEventBridgeVTK create connections between VTk events and QT.
//! </description>

class mafEventBridgeVTKTest : public QObject {
    Q_OBJECT

private slots:

    /// Initialize test variables
    void initTestCase() {
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafVisualPipeVTKSurface);
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

    testInteractionManagerCustom *bridge = new testInteractionManagerCustom();
    vtkRenderWindow *renWin = vtkRenderWindow::New();
    vtkRenderer *renderer = vtkRenderer::New();
    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();

    //Create an Actor to be rendered
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(5);
    sphereSource->Update();
    vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
    vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);

    renderer->AddActor(sphereActor);
    renWin->AddRenderer(renderer);
    iren->SetRenderWindow(renWin);

    m_EventBridge->setInteractor(iren);

    renWin->Render();
    QTest::qSleep(2000);

    //Send some interaction events by VTK (left Button press picking the actor)
    iren->SetEventPosition(201,138);
    iren->InvokeEvent(vtkCommand::LeftButtonPressEvent, NULL);

    //Create a sphere on the picking position
    vtkSmartPointer<vtkSphereSource> pickSphere = vtkSmartPointer<vtkSphereSource>::New();
    pickSphere->Update();
    vtkSmartPointer<vtkPolyDataMapper> pickSphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    pickSphereMapper->SetInputConnection(pickSphere->GetOutputPort());
    vtkSmartPointer<vtkActor> pickSphereActor = vtkSmartPointer<vtkActor>::New();
    pickSphereActor->SetMapper(pickSphereMapper);
    double *pos;
    pos = bridge->position();

    pickSphereActor->GetProperty()->SetColor(1,0,0);
    pickSphere->SetCenter(bridge->position());
    pickSphere->SetRadius(1);
    pickSphere->Update();
    renderer->AddActor(pickSphereActor);
    renWin->Render();
    QTest::qSleep(2000);




    //Check if events has been captured by testInteractionManagerCustom
    QVERIFY(m_CustomManager->m_Counter == 2);
}

MAF_REGISTER_TEST(mafEventBridgeVTKTest);
#include "mafEventBridgeVTKTest.moc"
