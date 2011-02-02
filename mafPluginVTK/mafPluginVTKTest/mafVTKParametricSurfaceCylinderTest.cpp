/*
 *  mafVTKParametricSurfaceCylinderTest.cpp
 *  mafPluginVTKTest
 *
 *  Created by Roberto Mucci on 20/01/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafVisualPipeVTKSurface.h>
#include <mafVTKParametricSurfaceCylinder.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAlgorithmOutput.h>
#include <mafContainer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <QDebug>

using namespace mafResources;
using namespace mafEventBus;
using namespace mafPluginVTK;

/**
 Class name: mafVTKParametricSurfaceCylinderTest
 This class implements the test suite for mafVTKParametricSurfaceCylinder.
 */

//! <title>
//mafVTKParametricSurfaceCylinder
//! </title>
//! <description>
//mafVTKParametricSurfaceCylinder represent a VTK cylinder cylinder with modificable parametrs.
//! </description>

class mafVTKParametricSurfaceCylinderTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafVisualPipeVTKSurface);

        // Create the parametric sphere.
        m_ParametricCylinder = mafNEW(mafPluginVTK::mafVTKParametricSurfaceCylinder);

        //! <snippet>
        //// m_DataSourceContainer is the container of type vtkAlgorithmOutput
        //// to "wrap" the vtkPolyData just simply use the code below.
        m_DataSourceContainer = m_ParametricCylinder->output();;

        //Insert data into VME
        m_VME = mafNEW(mafResources::mafVME);
        m_DataSet = mafNEW(mafResources::mafDataSet);
        m_DataSet->setDataValue(&m_DataSourceContainer);
        m_VME->dataSetCollection()->insertItem(m_DataSet, 0);
        //! </snippet>
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_DataSet);
        mafDEL(m_VME);
        mafDEL(m_ParametricCylinder);
        mafMessageHandler::instance()->shutdown();
    }

    /// Test Set/Get method of tparametric cylinder
    void SetGetTest();


private:
    mafVTKParametricSurfaceCylinder *m_ParametricCylinder; ///< Parametric sphere.
    mafVME *m_VME; ///< Contain the only item vtkPolydata representing a cylinder.
    mafResources::mafDataSet *m_DataSet;
    mafContainer<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
};

void mafVTKParametricSurfaceCylinderTest::SetGetTest() {
    mafVisualPipeVTKSurface *pipe;
    pipe = mafNEW(mafPluginVTK::mafVisualPipeVTKSurface);
    pipe->setInput(m_VME);
    pipe->setProperty("scalarVisibility", 0);
    pipe->setProperty("immediateRendering", 1);
    pipe->createPipe();
    pipe->updatePipe();

    // Get the vtkActor from the visual pipe
    // And assign to a mafContainer
    mafContainer<vtkActor> *actor = mafContainerPointerTypeCast(vtkActor, pipe->output());
    QVERIFY(actor != NULL);

    vtkRenderWindow *renWin = vtkRenderWindow::New();
    vtkRenderer *renderer = vtkRenderer::New();
    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();

    // Connect the actor (contained into the container) with the renderer.
    renderer->AddActor(*actor);
    renWin->AddRenderer(renderer);
    iren->SetRenderWindow(renWin);

    renderer->SetBackground(0.1, 0.1, 0.1);
    renWin->SetSize(640, 480);
    renWin->SetPosition(400,0);

    renWin->Render();
    //iren->Start();
    QTest::qSleep(2000);

    double newCenter[3];
    newCenter[0] = 10;
    newCenter[1] = 5;
    newCenter[2] = 20.5;

    //Change parameters of the sphere.
    m_ParametricCylinder->setCenter(newCenter);
    m_ParametricCylinder->setProperty("cylinderHeight", 20);
    m_ParametricCylinder->setProperty("cylinderRadius", 10);
    m_ParametricCylinder->setProperty("cylinderRes", 10);

    //Check if parameters had changed.
    QCOMPARE(m_ParametricCylinder->property("cylinderHeight").toDouble(), 20.0);
    QCOMPARE(m_ParametricCylinder->property("cylinderRadius").toDouble(), 10.0);
    QCOMPARE(m_ParametricCylinder->property("cylinderRes").toDouble(), 10.0);

    QCOMPARE(m_ParametricCylinder->center()[0], 10.0);
    QCOMPARE(m_ParametricCylinder->center()[1], 5.0);
    QCOMPARE(m_ParametricCylinder->center()[2], 20.5);

    renWin->Delete();
    renderer->Delete();
    iren->Delete();
    mafDEL(pipe);
}


MAF_REGISTER_TEST(mafVTKParametricSurfaceCylinderTest);
#include "mafVTKParametricSurfaceCylinderTest.moc"

