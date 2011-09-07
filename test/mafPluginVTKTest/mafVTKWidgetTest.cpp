/*
 *  mafVTKWidgetTest.cpp
 *  mafPluginVTKTest
 *
 *  Created by Roberto Mucci on 26/03/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafVTKWidget.h>
#include <mafPipeVisualVTKSurface.h>
#include <mafEventBusManager.h>

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

using namespace mafEventBus;
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
    testInteractionManagerCustom(const QString code_location = "");

    ~testInteractionManagerCustom();

    int m_Counter; ///< Test variable;
    double m_Pos[3]; ///< Test variable;
    
public Q_SLOTS:
    
    /// called when left mouse button is pressed.
    void mousePress(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e);
    
    /// called when left mouse button is released.
    void mouseRelease(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e);
    
};

testInteractionManagerCustom::~testInteractionManagerCustom() {
    mafEventBus::mafEventBusManager::instance()->removeSignal(this, "maf.local.resources.interaction.vmePick");
}


testInteractionManagerCustom::testInteractionManagerCustom(QString code_location) : mafObjectBase(code_location) {
    m_Counter = 0;
}


void testInteractionManagerCustom::mousePress(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e) {
    Q_UNUSED(modifiers);
    ++m_Counter;
    qDebug() << "mousePress";
    
    m_Pos[0] = pos[0];
    m_Pos[1] = pos[1];
    m_Pos[2] = pos[2];
}

void testInteractionManagerCustom::mouseRelease(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e) {
    Q_UNUSED(modifiers);
    --m_Counter;
    qDebug() << "mouseRelease";
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

private Q_SLOTS:

    /// Initialize test variables
    void initTestCase() {
        
        //retrieve root
        mafObject *root;
        QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObject *, root);
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.root", mafEventTypeLocal, NULL, &ret_val);
        
        //select
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafObjectBase *, root));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.select", mafEventTypeLocal, &argList);

        
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKSurface);
        m_CustomManager = mafNEW(testInteractionManagerCustom);
        initializeGraphicResources();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_CustomManager);
        shutdownGraphicResources();
    }

    /// mafVTKWidgetTest allocation test case.
    void mafVTKWidgetAllocationTest();

    /// mafVTKWidgetTest event connection test case.
    void mafVTKWidgetConnectionTest();

    /// LeftButtonRelease event connection test case.
    void mafVTKWidgetLeftButtonReleaseTest();

    /// RightButtonPres event connection test case.
    void mafVTKWidgetRightButtonPressTest();

    /// RightButtonPress event connection test case.
    void mafVTKWidgetRightButtonReleaseTest();

    /// MiddleButtonPres event connection test case.
    void mafVTKWidgetMiddleButtonPressTest();

    /// MiddleButtonPress event connection test case.
    void mafVTKWidgetMiddleButtonReleaseTest();


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
    
    bool result(false);
    result = connect(m_VTKWidget, SIGNAL(mousePressSignal(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)), m_CustomManager, SLOT(mousePress(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)));
    result = connect(m_VTKWidget, SIGNAL(mouseReleaseSignal(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)), m_CustomManager, SLOT(mouseRelease(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)));
    result= connect(m_VTKWidget, SIGNAL(mouseMoveSignal(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)), m_CustomManager, SLOT(mouseMove(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)));

    m_Renderer = vtkRenderer::New();
    m_VTKWidget->GetRenderWindow()->AddRenderer(m_Renderer);

    m_Renderer->SetBackground(0.1, 0.1, 0.1);
    m_VTKWidget->update();
    w->show();
}

void mafVTKWidgetTest::shutdownGraphicResources() {
    m_Renderer->Delete();
    w->close();
}

void mafVTKWidgetTest::mafVTKWidgetAllocationTest() {
    QVERIFY(m_VTKWidget != NULL);
}

void mafVTKWidgetTest::mafVTKWidgetConnectionTest() {
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
    QVERIFY(m_CustomManager->m_Counter == 1);

    //Create a sphere on the picking position
    vtkSmartPointer<vtkSphereSource> pickSphere = vtkSmartPointer<vtkSphereSource>::New();
    pickSphere->Update();
    vtkSmartPointer<vtkPolyDataMapper> pickSphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    pickSphereMapper->SetInputConnection(pickSphere->GetOutputPort());
    vtkSmartPointer<vtkActor> pickSphereActor = vtkSmartPointer<vtkActor>::New();
    pickSphereActor->SetMapper(pickSphereMapper);

    pickSphereActor->GetProperty()->SetColor(1,0,0);
    pickSphere->SetCenter(m_CustomManager->m_Pos);
    pickSphere->SetRadius(0.1);
    pickSphere->Update();
    m_Renderer->AddActor(pickSphereActor);
    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(2000);
}

void mafVTKWidgetTest::mafVTKWidgetLeftButtonReleaseTest() {
    QTestEventList events;
    QPoint point = QPoint(1200000, 50);
    events.addMouseRelease(Qt::LeftButton, 0, point);
    events.simulate(m_VTKWidget);
    QVERIFY(m_CustomManager->m_Counter == 0);
}

void mafVTKWidgetTest::mafVTKWidgetRightButtonPressTest() {
    QTestEventList events;
    QPoint point = QPoint(1200000, 50);
    events.addMousePress(Qt::RightButton, 0, point);
    events.simulate(m_VTKWidget);
    QVERIFY(m_CustomManager->m_Counter == 1);
}

void mafVTKWidgetTest::mafVTKWidgetRightButtonReleaseTest() {
    QTestEventList events;
    QPoint point = QPoint(12000000, 50);
    events.addMouseRelease(Qt::RightButton, 0, point);
    events.simulate(m_VTKWidget);
    QVERIFY(m_CustomManager->m_Counter == 0);
}

void mafVTKWidgetTest::mafVTKWidgetMiddleButtonPressTest() {
    QTestEventList events;
    QPoint point = QPoint(1200000, 50);
    events.addMousePress(Qt::MidButton, 0, point);
    events.simulate(m_VTKWidget);
    QVERIFY(m_CustomManager->m_Counter == 1);
}

void mafVTKWidgetTest::mafVTKWidgetMiddleButtonReleaseTest() {
    QTestEventList events;
    QPoint point = QPoint(1200000, 50);
    events.addMouseRelease(Qt::MidButton, 0, point);
    events.simulate(m_VTKWidget);
    QVERIFY(m_CustomManager->m_Counter == 0);
}

MAF_REGISTER_TEST(mafVTKWidgetTest);
#include "mafVTKWidgetTest.moc"
