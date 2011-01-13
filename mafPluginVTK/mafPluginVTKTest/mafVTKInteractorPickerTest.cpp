/*
 *  mafVTKInteractorPickerTest.cpp
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
#include <mafVTKInteractorPicker.h>

#include <mafContainer.h>
#include <vtkAlgorithmOutput.h>
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
using namespace mafEventBus;
using namespace mafPluginVTK;

/**
 Class name: mafVTKInteractorPickerTest
 This class implements the test suite for mafVTKWidget.
 */

//! <title>
//mafVTKInteractorPicker
//! </title>
//! <description>
//mafVTKInteractorPicker represent an interactor implementing a picking operation.
//! </description>

class mafVTKInteractorPickerTest : public QObject {
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

        m_Picker = mafNEW(mafPluginVTK::mafVTKInteractorPicker);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_VTKWidget);
        mafDEL(m_Picker);
        shutdownGraphicResources();
    }

    /// mafVTKInteractorPickerTest allocation test case.
    void mafVTKInteractorPickerAllocationTest();

    /// mafVTKInteractorPickerTest event connection test case.
    void mafVTKInteractorPickerEventsTest();

private:
    mafVTKWidget *m_VTKWidget; ///< Test var.
    mafVTKInteractorPicker *m_Picker; ///< Test var.

    vtkRenderer *m_Renderer; ///< Accessory renderer

    QMainWindow *w;

};

void mafVTKInteractorPickerTest::initializeGraphicResources() {
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

void mafVTKInteractorPickerTest::shutdownGraphicResources() {
    m_Renderer->Delete();
    delete m_VTKWidget;
    delete w;
}

void mafVTKInteractorPickerTest::mafVTKInteractorPickerAllocationTest() {
    QVERIFY(m_Picker != NULL);
}

void mafVTKInteractorPickerTest::mafVTKInteractorPickerEventsTest() {
    mafResourcesRegistration::registerResourcesObjects();
    // Create a polydata.
    vtkSphereSource *dataSource = vtkSphereSource::New();
    dataSource->SetRadius(10);
    dataSource->SetPhiResolution(20);
    dataSource->SetThetaResolution(20);
    dataSource->Update();

    //! <snippet>
    //// Create a container with the outputPort of a vtkCubeSource
    //// m_DataSourceContainer is the container of type vtkAlgorithmOutput
    //// to "wrap" the 'vtkCubeSource' of type vtkPolyData just simply use the code below.
    mafContainer<vtkAlgorithmOutput> dataSourceContainer;
    dataSourceContainer = dataSource->GetOutputPort(0);

    //Insert data into VME
    mafVME *vme = mafNEW(mafResources::mafVME);
    mafDataSet *dataSetSphere = mafNEW(mafResources::mafDataSet);
    dataSetSphere->setDataValue(&dataSourceContainer);
    vme->dataSetCollection()->insertItem(dataSetSphere, 0);
    vme->setInteractor(m_Picker);

    vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection(dataSource->GetOutputPort());
    vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);
    m_Renderer->AddActor(sphereActor);
    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //picking the actor
    QTestEventList events;
    events.addMousePress(Qt::LeftButton);
    events.simulate(m_VTKWidget);

    mafContainer<vtkActor> *actor = mafContainerPointerTypeCast(vtkActor, m_Picker->output());
    QVERIFY(actor != NULL);
    m_Renderer->AddActor(*actor);
    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //picking the actor in another point
    events.clear();
    QPoint point = QPoint(200, 200);
    events.addMousePress(Qt::LeftButton, 0, point);
    events.simulate(m_VTKWidget);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //Simulate operation event "Next pick".
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.operation.VTK.nextPick", mafEventTypeLocal, &argList);

    //picking the actor in another point after "Next Pick".
    events.clear();
    point = QPoint(200, 250);
    events.addMousePress(Qt::LeftButton, 0, point);
    events.simulate(m_VTKWidget);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //picking the actor in another point
    events.clear();
    point = QPoint(300, 200);
    events.addMousePress(Qt::LeftButton, 0, point);
    events.simulate(m_VTKWidget);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //Simulate operation event "Next pick".
    mafEventBusManager::instance()->notifyEvent("maf.local.operation.VTK.nextPick", mafEventTypeLocal, &argList);

    //picking the actor in another point
    events.clear();
    point = QPoint(350, 250);
    events.addMousePress(Qt::LeftButton, 0, point);
    events.simulate(m_VTKWidget);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //remove a pick marker picking the sphere with ctrl modifier
    events.clear();
    point = QPoint(320, 220);
    events.addMousePress(Qt::LeftButton, Qt::ControlModifier, point);
    events.simulate(m_VTKWidget);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //Simulate operation event "Next pick".
    mafEventBusManager::instance()->notifyEvent("maf.local.operation.VTK.nextPick", mafEventTypeLocal, &argList);

    //picking the actor in another point
    events.clear();
    point = QPoint(380, 220);
    events.addMousePress(Qt::LeftButton, 0, point);
    events.simulate(m_VTKWidget);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //remove a pick marker picking the sphere with ctrl modifier
    events.clear();
    point = QPoint(375, 215);
    events.addMousePress(Qt::LeftButton, Qt::ControlModifier, point);
    events.simulate(m_VTKWidget);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //picking the actor in another point
    events.clear();
    point = QPoint(400, 210);
    events.addMousePress(Qt::LeftButton, 0, point);
    events.simulate(m_VTKWidget);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);


    //Simulate operation event "Undo pick".
    /*mafEventArgumentsList argList1;
    mafEventBusManager::instance()->notifyEvent("maf.local.operation.VTK.unDoPick", mafEventTypeLocal, &argList1);

    mafContainer<vtkActor> *actor6 = mafContainerPointerTypeCast(vtkActor, m_Picker->output());
    QVERIFY(actor6 != NULL);
    m_Renderer->AddActor(*actor6);
    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);*/
}


MAF_REGISTER_TEST(mafVTKInteractorPickerTest);
#include "mafVTKInteractorPickerTest.moc"

