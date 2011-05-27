/*
 *  mafVTKParametricSurfaceCubeTest.cpp
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
#include <mafPipeVisualVTKSurface.h>
#include <mafVTKParametricSurfaceCube.h>
#include <mafVTKWidget.h>
#include <vtkAlgorithmOutput.h>
#include <mafProxy.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <QDebug>

using namespace mafResources;
using namespace mafEventBus;
using namespace mafPluginVTK;

/**
 Class name: mafVTKParametricSurfaceCubeTest
 This class implements the test suite for mafVTKParametricSurfaceCube.
 */

//! <title>
//mafVTKParametricSurfaceCube
//! </title>
//! <description>
//mafVTKParametricSurfaceCube represent a VTK cube surface with modificable parameters.
//! </description>

class mafVTKParametricSurfaceCubeTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKSurface);
        
        m_RenderWidget = new mafVTKWidget();

        // Create the parametric sphere.
        m_ParametricCube = mafNEW(mafPluginVTK::mafVTKParametricSurfaceCube);

        //! <snippet>
        //// m_DataSourceContainer is the container of type vtkAlgorithmOutput
        //// to "wrap" the vtkPolyData just simply use the code below.
        m_DataSourceContainer.setClassTypeNameFunction(vtkClassTypeNameExtract);
        m_DataSourceContainer = m_ParametricCube->output();

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
        mafDEL(m_ParametricCube);
        mafMessageHandler::instance()->shutdown();
        delete m_RenderWidget;
    }

    /// Test Set/Get method of tparametric surface
    void SetGetTest();


private:
    mafVTKParametricSurfaceCube *m_ParametricCube; ///< Parametric sphere.
    mafVME *m_VME; ///< Contain the only item vtkPolydata representing a surface.
    mafResources::mafDataSet *m_DataSet;
    mafProxy<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
    QObject *m_RenderWidget; /// renderer widget
};

void mafVTKParametricSurfaceCubeTest::SetGetTest() {
    vtkRenderer *renderer = vtkRenderer::New();
    vtkRenderWindow *renWin = ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow();

    mafPipeVisualVTKSurface *pipe;
    pipe = mafNEW(mafPluginVTK::mafPipeVisualVTKSurface);
    pipe->setInput(m_VME);
    pipe->setProperty("scalarVisibility", 0);
    pipe->setProperty("immediateRendering", 1);
    pipe->setGraphicObject(m_RenderWidget);
    pipe->createPipe();
    pipe->updatePipe();

    // Get the vtkActor from the visual pipe
    // And assign to a mafProxy
    mafProxy<vtkActor> *actor = mafProxyPointerTypeCast(vtkActor, pipe->output());
    QVERIFY(actor != NULL);

    // Connect the actor (contained into the container) with the renderer.
    renderer->AddActor(*actor);
    renWin->AddRenderer(renderer);

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

    //Change parameters of the cube.
    m_ParametricCube->setCenter(newCenter);
    m_ParametricCube->setProperty("cubeXLength", 15);
    m_ParametricCube->setProperty("cubeYLength", 20);
    m_ParametricCube->setProperty("cubeZLength", 5);
    m_ParametricCube->updateSurface();

    //Check if parameters had changed.
    QCOMPARE(m_ParametricCube->property("cubeXLength").toDouble(), 15.0);
    QCOMPARE(m_ParametricCube->property("cubeYLength").toDouble(), 20.0);
    QCOMPARE(m_ParametricCube->property("cubeZLength").toDouble(), 5.0);

    QCOMPARE(m_ParametricCube->center()[0], 10.0);
    QCOMPARE(m_ParametricCube->center()[1], 5.0);
    QCOMPARE(m_ParametricCube->center()[2], 20.5);

    renderer->Delete();
    pipe->setGraphicObject(NULL);
    mafDEL(pipe);
}


MAF_REGISTER_TEST(mafVTKParametricSurfaceCubeTest);
#include "mafVTKParametricSurfaceCubeTest.moc"
