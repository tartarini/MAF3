/*
 *  mafInteractorVTKCameraTest.cpp
 *  mafPluginVTKTest
 *
 *  Created by Roberto Mucci on 26/03/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
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
#include <mafInteractorVTKCamera.h>
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
 Class name: mafInteractorVTKCameraTest
 This class implements the test suite for mafInteractorVTKCamera.
 */

//! <title>
//mafInteractorVTKCamera
//! </title>
//! <description>
//mafInteractorVTKCamera represent an interactor implementing a picking operation.
//! </description>

class mafInteractorVTKCameraTest : public QObject {
    Q_OBJECT
    //initialize all the graphic resources
    void initializeGraphicResources();

    //shutdown all the graphic resources
    void shutdownGraphicResources();

private Q_SLOTS:

    /// Initialize test variables
    void initTestCase() {
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKSurface);
        initializeGraphicResources();

        m_Picker = mafNEW(mafPluginVTK::mafInteractorVTKCamera);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Picker);
        shutdownGraphicResources();
    }

    /// mafInteractorVTKCameraTest allocation test case.
    void mafInteractorVTKCameraAllocationTest();

    /// mafInteractorVTKCameraTest event connection test case.
    void mafInteractorVTKCameraEventsTest();

private:
    mafVTKWidget *m_VTKWidget; ///< Test var.
    mafInteractorVTKCamera *m_Picker; ///< Test var.

    vtkRenderer *m_Renderer; ///< Accessory renderer

    QMainWindow *w;

};

void mafInteractorVTKCameraTest::initializeGraphicResources() {
    w = new QMainWindow();
    w->setMinimumSize(640,480);

    m_VTKWidget = new mafVTKWidget();
    m_VTKWidget->setParent(w);

    m_Renderer = m_VTKWidget->renderer();

    m_Renderer->SetBackground(0.1, 0.1, 0.1);
    m_VTKWidget->update();
    w->show();
}

void mafInteractorVTKCameraTest::shutdownGraphicResources() {
    w->close();
}

void mafInteractorVTKCameraTest::mafInteractorVTKCameraAllocationTest() {
    QVERIFY(m_Picker != NULL);
}

void mafInteractorVTKCameraTest::mafInteractorVTKCameraEventsTest() {
}


MAF_REGISTER_TEST(mafInteractorVTKCameraTest);
#include "mafInteractorVTKCameraTest.moc"

