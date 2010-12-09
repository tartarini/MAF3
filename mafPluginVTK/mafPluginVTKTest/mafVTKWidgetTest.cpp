/*
 *  mafVTKWidgetTest.cpp
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
#include <mafVTKWidget.h>
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
     QVERIFY((modifiers&(1<<MAF_CTRL_KEY))==0);
     QVERIFY((modifiers&(1<<MAF_ALT_KEY))==0);
}
double *testInteractionManagerCustom::position() {
    return m_Pos;
}


/**
 Class name: mafVTKWidgetTest
 This class implements the test suite for mafVTKWidget.
 */

//! <title>
//mafVTKWidget
//! </title>
//! <description>
//mafVTKWidget overloads VKT mouse events and forward them to EventBus.
//! </description>

class mafVTKWidgetTest : public QObject {
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
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_CustomManager);
        mafDEL(m_VTKWidget);
        shutdownGraphicResources();
    }

    /// mafVTKWidgetTest allocation test case.
    void mafEventBridgeVTKAllocationTest();

    /// mafVTKWidgetTest event connection test case.
    void mafEventBridgeVTKConnectionTest();

    /// LeftButtonRelease event connection test case.
    void mafEventBridgeVTKLeftButtonReleaseTest();

    /// RightButtonPres event connection test case.
    void mafEventBridgeVTKRightButtonPressTest();

    /// RightButtonPress event connection test case.
    void mafEventBridgeVTKRightButtonReleaseTest();

    /// MiddleButtonPres event connection test case.
    void mafEventBridgeVTKMiddleButtonPressTest();

    /// MiddleButtonPress event connection test case.
    void mafEventBridgeVTKMiddleButtonReleaseTest();


private:
    testInteractionManagerCustom *m_CustomManager; ///< Test var.
    mafVTKWidget *m_VTKWidget; ///< Test var.

    vtkRenderer *m_Renderer; ///< Accessory renderer

    QMainWindow *w;
};

void mafVTKWidgetTest::initializeGraphicResources() {
    w = new QMainWindow();
    w->setMinimumSize(640,480);

    m_VTKWidget = new mafVTKWidget();
    m_VTKWidget->setParent(w);

    m_Renderer = vtkRenderer::New();
    m_VTKWidget->GetRenderWindow()->AddRenderer(m_Renderer);

    m_Renderer->SetBackground(0.1, 0.1, 0.1);
    m_VTKWidget->update();
    w->show();
}

void mafVTKWidgetTest::shutdownGraphicResources() {

    //m_Renderer->Delete();
    delete m_VTKWidget;
}

void mafVTKWidgetTest::mafEventBridgeVTKAllocationTest() {
    QVERIFY(m_VTKWidget != NULL);
}

void mafVTKWidgetTest::mafEventBridgeVTKConnectionTest() {
    //Create an Actor to be rendered
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(1);
    sphereSource->Update();
    vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
    vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);
    m_Renderer->AddActor(sphereActor);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(2000);

    //Send some interaction events by VTK (left Button press picking the actor)

    QTestEventList events;
    events.addMousePress(Qt::LeftButton, Qt::ShiftModifier);
    events.simulate(m_VTKWidget);
    //Send some interaction events by VTK (left Button press picking the actor)

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
    pickSphere->SetRadius(0.1);
    pickSphere->Update();
    m_Renderer->AddActor(pickSphereActor);
    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(2000);

    //Check if events has been captured by testInteractionManagerCustom
    QVERIFY(m_CustomManager->m_Counter == 2);
    m_CustomManager->m_Counter = 0;

}

void mafVTKWidgetTest::mafEventBridgeVTKLeftButtonReleaseTest() {
    QTestEventList events;
    QPoint point = QPoint(120, 50);
    events.addMouseRelease(Qt::LeftButton, 0, point);
    events.simulate(m_VTKWidget);

    QVERIFY(m_CustomManager->m_Counter == 1);
    m_CustomManager->m_Counter = 0;
}

void mafVTKWidgetTest::mafEventBridgeVTKRightButtonPressTest() {
    QTestEventList events;
    QPoint point = QPoint(120, 50);
    events.addMousePress(Qt::RightButton, 0, point);
    events.simulate(m_VTKWidget);

    QVERIFY(m_CustomManager->m_Counter == 1);
    m_CustomManager->m_Counter = 0;
}

void mafVTKWidgetTest::mafEventBridgeVTKRightButtonReleaseTest() {
    QTestEventList events;
    QPoint point = QPoint(120, 50);
    events.addMouseRelease(Qt::RightButton, 0, point);
    events.simulate(m_VTKWidget);

    QVERIFY(m_CustomManager->m_Counter == 1);
    m_CustomManager->m_Counter = 0;
}

void mafVTKWidgetTest::mafEventBridgeVTKMiddleButtonPressTest() {
    QTestEventList events;
    QPoint point = QPoint(120, 50);
    events.addMousePress(Qt::MidButton, 0, point);
    events.simulate(m_VTKWidget);

    QVERIFY(m_CustomManager->m_Counter == 1);
    m_CustomManager->m_Counter = 0;
}

void mafVTKWidgetTest::mafEventBridgeVTKMiddleButtonReleaseTest() {
    QTestEventList events;
    QPoint point = QPoint(120, 50);
    events.addMouseRelease(Qt::MidButton, 0, point);
    events.simulate(m_VTKWidget);

    QVERIFY(m_CustomManager->m_Counter == 1);
    m_CustomManager->m_Counter = 0;
}

MAF_REGISTER_TEST(mafVTKWidgetTest);
#include "mafVTKWidgetTest.moc"
