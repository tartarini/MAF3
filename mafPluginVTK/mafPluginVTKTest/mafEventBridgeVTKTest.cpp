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
#include <QDebug>
#include <QVTKWidget.h>
#include <QMainWindow>

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
    void leftButtonPress(unsigned long modifiers);

    /// observer needed to receive the 'maf.local.resources.interaction.leftButtonRelease' signal
    void leftButtonRelease(unsigned long modifiers);

    /// observer needed to receive the 'maf.local.resources.interaction.rightButtonRelease' signal
    void rightButtonPress(unsigned long modifiers);

    /// observer needed to receive the 'maf.local.resources.interaction.rightButtonRelease' signal
    void rightButtonRelease(unsigned long modifiers);

    /// observer needed to receive the 'maf.local.resources.interaction.middleButtonRelease' signal
    void middleButtonPress(unsigned long modifiers);

    /// observer needed to receive the 'maf.local.resources.interaction.middleButtonRelease' signal
    void middleButtonRelease(unsigned long modifiers);

    /// observer needed to receive the 'maf.local.resources.interaction.vmePick' signal
    void pick(double *pos, unsigned long modifiers, mafCore::mafContainerInterface *interface);

signals:
    /// left button pressed.
    void leftButtonPressSignal(unsigned long modifiers);

    /// left button relesed.
    void leftButtonReleaseSignal(unsigned long modifiers);

    /// right button press.
    void rightButtonPressSignal(unsigned long modifiers);

    /// right button relesed.
    void rightButtonReleaseSignal(unsigned long modifiers);

    /// middle button press.
    void middleButtonPressSignal(unsigned long modifiers);

    /// middle button relesed.
    void middleButtonReleaseSignal(unsigned long modifiers);

    /// picked button pressed.
    void pickSignal(double *pos, unsigned long modifiers, mafCore::mafContainerInterface *interface);

};

testInteractionManagerCustom::testInteractionManagerCustom(QString code_location) : mafObjectBase(code_location) {
    mafRegisterLocalSignal("maf.local.resources.interaction.leftButtonPress", this, "leftButtonPressSignal(unsigned long)");
    mafRegisterLocalCallback("maf.local.resources.interaction.leftButtonPress", this, "leftButtonPress(unsigned long)");

    mafRegisterLocalSignal("maf.local.resources.interaction.leftButtonRelease", this, "leftButtonReleaseSignal(unsigned long)");
    mafRegisterLocalCallback("maf.local.resources.interaction.leftButtonRelease", this, "leftButtonRelease(unsigned long)");

    mafRegisterLocalSignal("maf.local.resources.interaction.rightButtonPress", this, "rightButtonPressSignal(unsigned long)");
    mafRegisterLocalCallback("maf.local.resources.interaction.rightButtonPress", this, "rightButtonPress(unsigned long)");

    mafRegisterLocalSignal("maf.local.resources.interaction.rightButtonRelease", this, "rightButtonReleaseSignal(unsigned long)");
    mafRegisterLocalCallback("maf.local.resources.interaction.rightButtonRelease", this, "rightButtonRelease(unsigned long)");

    mafRegisterLocalSignal("maf.local.resources.interaction.middleButtonPress", this, "middleButtonPressSignal(unsigned long)");
    mafRegisterLocalCallback("maf.local.resources.interaction.middleButtonPress", this, "middleButtonPress(unsigned long)");

    mafRegisterLocalSignal("maf.local.resources.interaction.middleButtonRelease", this, "middleButtonReleaseSignal(unsigned long)");
    mafRegisterLocalCallback("maf.local.resources.interaction.middleButtonRelease", this, "middleButtonRelease(unsigned long)");

    mafRegisterLocalSignal("maf.local.resources.interaction.vmePick", this, "pickSignal(double *, unsigned long, mafCore::mafContainerInterface *)");
    mafRegisterLocalCallback("maf.local.resources.interaction.vmePick", this, "pick(double *, unsigned long, mafCore::mafContainerInterface *)");
    m_Counter = 0;
}

void testInteractionManagerCustom::leftButtonPress(unsigned long modifiers) {
    m_Counter++;
    qDebug() << "leftButtonPress";
}

void testInteractionManagerCustom::leftButtonRelease(unsigned long modifiers) {
    m_Counter++;
    qDebug() << "leftButtonRelease";
}

void testInteractionManagerCustom::rightButtonPress(unsigned long modifiers) {
    m_Counter++;
    qDebug() << "rightButtonPress";
}

void testInteractionManagerCustom::rightButtonRelease(unsigned long modifiers) {
    m_Counter++;
    qDebug() << "rightButtonRelease";
}

void testInteractionManagerCustom::middleButtonPress(unsigned long modifiers) {
    m_Counter++;
    qDebug() << "middleButtonPress";
}

void testInteractionManagerCustom::middleButtonRelease(unsigned long modifiers) {
    m_Counter++;
    qDebug() << "middleButtonRelease";
}

void testInteractionManagerCustom::pick(double *pos, unsigned long modifiers,  mafCore::mafContainerInterface *interface) {
     m_Counter++;
     m_Pos[0] = pos[0];
     m_Pos[1]= pos[1];
     m_Pos[2] = pos[2];

     // check for ctrl and shift pressure
     QVERIFY((modifiers&(1<<MAF_SHIFT_KEY))!=0);
     QVERIFY((modifiers&(1<<MAF_CTRL_KEY))!=0);
     QVERIFY((modifiers&(1<<MAF_ALT_KEY))==0);
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
    //initialize all the graphic resources
    void initializeGraphicResources();

    //shutdown all the graphic resources
    void shutdownGraphicResources();

private slots:

    /// Initialize test variables
    void initTestCase() {
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafVisualPipeVTKSurface);
        initializeGraphicResources();
        m_CustomManager = mafNEW(testInteractionManagerCustom);
        m_EventBridge = mafNEW(mafPluginVTK::mafEventBridgeVTK);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_CustomManager);
        mafDEL(m_EventBridge);
        shutdownGraphicResources();
    }

    /// mafEventBridgeVTKTest allocation test case.
    void mafEventBridgeVTKAllocationTest();

    /// mafEventBridgeVTKTest event connection test case.
    void mafEventBridgeVTKConnectionTest();

    /// LeftButtonRelease event connection test case.
    void mafEventBridgeVTKLeftButtonReleaseTest();

    /// LeftButtonPress event connection test case.
    void mafEventBridgeVTKLeftButtonPressTest();

    /// RightButtonPres event connection test case.
    void mafEventBridgeVTKRightButtonPressTest();

    /// RightButtonPress event connection test case.
    void mafEventBridgeVTKRightButtonReleaseTest();

    /// MiddleButtonPres event connection test case.
    void mafEventBridgeVTKMiddleButtonPressTest();

    /// MiddleButtonPress event connection test case.
    void mafEventBridgeVTKMiddleButtonReleaseTest();


private:
    mafEventBridgeVTK *eventBridge; ///< Test var.
    testInteractionManagerCustom *m_CustomManager; ///< Test var.
    mafEventBridgeVTK *m_EventBridge; ///< Test var.

    vtkRenderWindow *m_RenWin; ///< Accessory render window to show the rendered data
    vtkRenderer *m_Renderer; ///< Accessory renderer
    vtkRenderWindowInteractor *m_Iren; ///< Accessory interactor.

    QVTKWidget *m_Widget;
    QMainWindow *w;
};

void mafEventBridgeVTKTest::initializeGraphicResources() {
    w = new QMainWindow();

    m_Widget = new QVTKWidget();
    m_Widget->setParent(w);

    m_Renderer = vtkRenderer::New();
    m_Widget->GetRenderWindow()->AddRenderer(m_Renderer);

    m_RenWin = m_Widget->GetRenderWindow();
    //m_RenWin = vtkRenderWindow::New();

    m_Iren = vtkRenderWindowInteractor::New();
    m_RenWin->AddRenderer(m_Renderer);
    m_Iren->SetRenderWindow(m_RenWin);

    m_Renderer->SetBackground(0.1, 0.1, 0.1);
    m_RenWin->SetSize(640, 480);
    m_RenWin->SetPosition(200,0);
    w->show();
}

void mafEventBridgeVTKTest::shutdownGraphicResources() {

    //m_RenWin->Delete();
    //m_Renderer->Delete();
    //m_Iren->Delete();
    delete m_Widget;
}

void mafEventBridgeVTKTest::mafEventBridgeVTKAllocationTest() {
    QVERIFY(m_EventBridge != NULL);
    m_EventBridge->setInteractor(m_Iren);
}

void mafEventBridgeVTKTest::mafEventBridgeVTKConnectionTest() {
    //Create an Actor to be rendered
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(5);
    sphereSource->Update();
    vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
    vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);
    m_Renderer->AddActor(sphereActor);

    m_RenWin->Render();
    QTest::qSleep(2000);

    //Send some interaction events by VTK (left Button press picking the actor)

    //m_Iren->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, NULL);


    //Create a sphere on the picking position
    vtkSmartPointer<vtkSphereSource> pickSphere = vtkSmartPointer<vtkSphereSource>::New();
    pickSphere->Update();
    vtkSmartPointer<vtkPolyDataMapper> pickSphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    pickSphereMapper->SetInputConnection(pickSphere->GetOutputPort());
    vtkSmartPointer<vtkActor> pickSphereActor = vtkSmartPointer<vtkActor>::New();
    pickSphereActor->SetMapper(pickSphereMapper);
    double *pos;
    pos = m_CustomManager->position();

    pickSphereActor->GetProperty()->SetColor(1,0,0);
    pickSphere->SetCenter(m_CustomManager->position());
    pickSphere->SetRadius(1);
    pickSphere->Update();
    m_Renderer->AddActor(pickSphereActor);
    m_RenWin->Render();
    QTest::qSleep(2000);

    //Check if events has been captured by testInteractionManagerCustom
    //QVERIFY(m_CustomManager->m_Counter == 1);
    m_CustomManager->m_Counter = 0;

}

void mafEventBridgeVTKTest::mafEventBridgeVTKLeftButtonReleaseTest() {
    //m_Iren->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, NULL);
    QTestEventList events;
    events.addMouseRelease(Qt::LeftButton);
    events.simulate(m_Widget);

    qDebug() << m_CustomManager->m_Counter;
    QVERIFY(m_CustomManager->m_Counter == 1);
    m_CustomManager->m_Counter = 0;
}

void mafEventBridgeVTKTest::mafEventBridgeVTKLeftButtonPressTest() {
    QTestEventList events;
    events.addMousePress(Qt::LeftButton);
    events.simulate(m_Widget);

    qDebug() << m_CustomManager->m_Counter;
    QVERIFY(m_CustomManager->m_Counter == 1);
    m_CustomManager->m_Counter = 0;
}

void mafEventBridgeVTKTest::mafEventBridgeVTKRightButtonPressTest() {
    QTestEventList events;
    events.addMousePress(Qt::RightButton);
    events.simulate(m_Widget);

    qDebug() << m_CustomManager->m_Counter;
    QVERIFY(m_CustomManager->m_Counter == 1);
    m_CustomManager->m_Counter = 0;
}

void mafEventBridgeVTKTest::mafEventBridgeVTKRightButtonReleaseTest() {
    QTestEventList events;
    events.addMouseRelease(Qt::RightButton);
    events.simulate(m_Widget);

    qDebug() << m_CustomManager->m_Counter;
    QVERIFY(m_CustomManager->m_Counter == 1);
    m_CustomManager->m_Counter = 0;
}

void mafEventBridgeVTKTest::mafEventBridgeVTKMiddleButtonPressTest() {
    QTestEventList events;
    events.addMousePress(Qt::MidButton);
    events.simulate(m_Widget);

    qDebug() << m_CustomManager->m_Counter;
    QVERIFY(m_CustomManager->m_Counter == 1);
    m_CustomManager->m_Counter = 0;
}

void mafEventBridgeVTKTest::mafEventBridgeVTKMiddleButtonReleaseTest() {
    QTestEventList events;
    events.addMouseRelease(Qt::MidButton);
    events.simulate(m_Widget);

    qDebug() << m_CustomManager->m_Counter;
    QVERIFY(m_CustomManager->m_Counter == 1);
    m_CustomManager->m_Counter = 0;
}

//MAF_REGISTER_TEST(mafEventBridgeVTKTest);
#include "mafEventBridgeVTKTest.moc"
