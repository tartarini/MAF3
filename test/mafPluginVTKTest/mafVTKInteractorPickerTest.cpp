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
#include <mafPipeVisualVTKSurface.h>
#include <mafDataBoundaryAlgorithmVTK.h>
#include <mafVTKInteractorPicker.h>
#include <mafVTKParametricSurfaceSphere.h>

#include <mafProxy.h>
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
#include <vtkRendererCollection.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <QDebug>
#include <QVTKWidget.h>
#include <QMainWindow>

using namespace mafResources;
using namespace mafEventBus;
using namespace mafPluginVTK;

/**
 Class name: mafVTKInteractorPickerTest
 This class implements the test suite for mafVTKInteractorPicker.
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
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKSurface);
        initializeGraphicResources();

        m_Picker = mafNEW(mafPluginVTK::mafVTKInteractorPicker);
        
        //retrieve root
        mafObject *root;
        QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObject *, root);
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.root", mafEventTypeLocal, NULL, &ret_val);
        
        //select
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafObjectBase *, root));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.select", mafEventTypeLocal, &argList);

    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
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
    w->close();
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
    mafProxy<vtkAlgorithmOutput> dataSourceContainer;
    dataSourceContainer.setClassTypeNameFunction(vtkClassTypeNameExtract);
    dataSourceContainer = dataSource->GetOutputPort(0);
    //! </snippet>

    //Insert data into VME
    mafVME *vme = mafNEW(mafResources::mafVME);
    mafDataSet *dataSetSphere = mafNEW(mafResources::mafDataSet);
    mafDataBoundaryAlgorithmVTK *boundaryAlgorithm;
    boundaryAlgorithm = mafNEW(mafDataBoundaryAlgorithmVTK);
    dataSetSphere->setBoundaryAlgorithm(boundaryAlgorithm);
    dataSetSphere->setDataValue(&dataSourceContainer);
    vme->dataSetCollection()->insertItem(dataSetSphere, 0);
    mafDEL(dataSetSphere);
    vme->pushInteractor(m_Picker);

    //Must create a visual pipe that send "maf.local.resources.interaction.vmePicked"
    //when its vme is picked
    mafPipeVisualVTKSurface *pipe;
    pipe = mafNEW(mafPluginVTK::mafPipeVisualVTKSurface);
    pipe->setInput(vme);
    pipe->setGraphicObject(m_VTKWidget);
    pipe->updatePipe();

    mafProxy<vtkActor> *sphereActor = mafProxyPointerTypeCast(vtkActor, pipe->output());
    m_Renderer->AddActor(*sphereActor);
    m_VTKWidget->GetRenderWindow()->Render();
    m_VTKWidget->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
    QTest::qSleep(1000);

    //Create parametric surface used by picker
    QString surfaceType = "mafPluginVTK::mafVTKParametricSurfaceSphere";
    m_Picker->setSurface(surfaceType);

    //picking the actor
    QTestEventList events;
    events.addMouseRelease(Qt::LeftButton);
    events.simulate(m_VTKWidget);

    vtkActor *actor = m_Picker->output();
    QVERIFY(actor != NULL);
    m_Renderer->AddActor(actor);
    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //set another interactor picker
    mafVTKInteractorPicker *picker = mafNEW(mafPluginVTK::mafVTKInteractorPicker);
    vme->pushInteractor(picker);

    //re attached old interactor picker
    vme->pushInteractor(m_Picker);

    //picking the actor in another point
    events.clear();
    QPoint point = QPoint(200, 200);
    events.addMouseRelease(Qt::LeftButton, 0, point);
    events.simulate(m_VTKWidget);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //Simulate operation event "Next pick".
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.operation.VTK.nextPick", mafEventTypeLocal, &argList);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //picking the actor in another point after "Next Pick".
    events.clear();
    point = QPoint(200, 250);
    events.addMouseRelease(Qt::LeftButton, 0, point);
    events.simulate(m_VTKWidget);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //picking the actor in another point
    events.clear();
    point = QPoint(300, 200);
    events.addMouseRelease(Qt::LeftButton, 0, point);
    events.simulate(m_VTKWidget);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //Simulate operation event "Next pick".
    mafEventBusManager::instance()->notifyEvent("maf.local.operation.VTK.nextPick", mafEventTypeLocal, &argList);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //picking the actor in another point
    events.clear();
    point = QPoint(350, 250);
    events.addMouseRelease(Qt::LeftButton, 0, point);
    events.simulate(m_VTKWidget);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //remove a pick marker picking the sphere with ctrl modifier
    events.clear();
    point = QPoint(320, 220);
    events.addMouseRelease(Qt::LeftButton, Qt::ControlModifier, point);
    events.simulate(m_VTKWidget);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //Simulate operation event "Next pick".
    mafEventBusManager::instance()->notifyEvent("maf.local.operation.VTK.nextPick", mafEventTypeLocal, &argList);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //picking the actor in another point
    events.clear();
    point = QPoint(380, 220);
    events.addMouseRelease(Qt::LeftButton, 0, point);
    events.simulate(m_VTKWidget);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //remove a pick marker picking the sphere with ctrl modifier
    events.clear();
    point = QPoint(375, 215);
    events.addMouseRelease(Qt::LeftButton, Qt::ControlModifier, point);
    events.simulate(m_VTKWidget);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    //picking the actor in another point
    events.clear();
    point = QPoint(400, 210);
    events.addMouseRelease(Qt::LeftButton, 0, point);
    events.simulate(m_VTKWidget);

    m_VTKWidget->GetRenderWindow()->Render();
    QTest::qSleep(1000);

    mafDEL(picker);
    pipe->setGraphicObject(NULL);
    mafDEL(pipe);
    mafDEL(vme);

}


MAF_REGISTER_TEST(mafVTKInteractorPickerTest);
#include "mafVTKInteractorPickerTest.moc"

