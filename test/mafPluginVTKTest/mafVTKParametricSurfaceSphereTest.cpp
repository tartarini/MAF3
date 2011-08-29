/*
 *  mafVTKParametricSurfaceSphereTest.cpp
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
#include <mafVTKParametricSurfaceSphere.h>
#include <mafVTKWidget.h>
#include <mafDataBoundaryAlgorithmVTK.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAlgorithmOutput.h>
#include <mafProxy.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <QMainWindow>
#include <QDebug>

using namespace mafResources;
using namespace mafEventBus;
using namespace mafPluginVTK;

/**
 Class name: mafVTKParametricSurfaceSphereTest
 This class implements the test suite for mafVTKParametricSurfaceSphere.
 */

//! <title>
//mafVTKParametricSurfaceSphere
//! </title>
//! <description>
//mafVTKParametricSurfaceSphere represent a VTK sphere surface with modificable parameters.
//! </description>

class mafVTKParametricSurfaceSphereTest : public QObject {
    Q_OBJECT
    //initialize all the graphic resources
    void initializeGraphicResources();

    //shutdown all the graphic resources
    void shutdownGraphicResources();

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKSurface);
        
        // Create the parametric sphere.
        m_ParametricSphere = mafNEW(mafPluginVTK::mafVTKParametricSurfaceSphere);

        //! <snippet>
        //// m_DataSourceContainer is the container of type vtkAlgorithmOutput
        //// to "wrap" the vtkPolyData just simply use the code below.
        m_DataSourceContainer.setClassTypeNameFunction(vtkClassTypeNameExtract);
        m_DataSourceContainer = m_ParametricSphere->output();

        //Insert data into VME
        m_VME = mafNEW(mafResources::mafVME);
        m_DataSet = mafNEW(mafResources::mafDataSet);
        mafDataBoundaryAlgorithmVTK *boundaryAlgorithm;
        boundaryAlgorithm = mafNEW(mafDataBoundaryAlgorithmVTK);
        m_DataSet->setBoundaryAlgorithm(boundaryAlgorithm);
        m_DataSet->setDataValue(&m_DataSourceContainer);
        m_VME->dataSetCollection()->insertItem(m_DataSet, 0);
        //! </snippet>
        initializeGraphicResources();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_DataSet);
        mafDEL(m_VME);
        mafDEL(m_ParametricSphere);
        mafMessageHandler::instance()->shutdown();
        shutdownGraphicResources();
    }

    /// visualize parametric surface
    void visualizeTest();


private:
    mafVTKParametricSurfaceSphere *m_ParametricSphere; ///< Parametric sphere.
    mafVME *m_VME; ///< Contain the only item vtkPolydata representing a surface.
    mafResources::mafDataSet *m_DataSet;
    mafProxy<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
    QObject *m_RenderWidget; /// renderer widget
    vtkRenderer *m_Renderer; ///< Accessory renderer
    QMainWindow *w;
};

void mafVTKParametricSurfaceSphereTest::initializeGraphicResources() {
    w = new QMainWindow();
    w->setMinimumSize(640,480);

    m_RenderWidget = new mafVTKWidget();
    ((mafVTKWidget*)m_RenderWidget)->setParent(w);

    m_Renderer = vtkRenderer::New();
    ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->AddRenderer(m_Renderer);

    m_Renderer->SetBackground(0.1, 0.1, 0.1);
    ((mafVTKWidget*)m_RenderWidget)->update();
    w->show();
}

void mafVTKParametricSurfaceSphereTest::shutdownGraphicResources() {
    m_Renderer->Delete();
    w->close();
}

void mafVTKParametricSurfaceSphereTest::visualizeTest() {
     mafPipeVisualVTKSurface *pipe;
    pipe = mafNEW(mafPluginVTK::mafPipeVisualVTKSurface);
    pipe->setInput(m_VME);
    pipe->setProperty("scalarVisibility", 0);
    pipe->setProperty("immediateRendering", 1);
    pipe->setGraphicObject(m_RenderWidget);
    pipe->updatePipe();

    // Get the vtkActor from the visual pipe
    // And assign to a mafProxy
    mafProxy<vtkActor> *actor = mafProxyPointerTypeCast(vtkActor, pipe->output());
    QVERIFY(actor != NULL);

    // Connect the actor (contained into the container) with the renderer.
    m_Renderer->AddActor(*actor);

    ((mafVTKWidget*)m_RenderWidget)->update();
    ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->Render();

    //iren->Start();
    QTest::qSleep(2000);

    double newCenter[3];
    newCenter[0] = 10;
    newCenter[1] = 5;
    newCenter[2] = 20.5;

    //Change parameters of the sphere.
    m_ParametricSphere->setCenter(newCenter);
    m_ParametricSphere->setProperty("sphereThetaRes", 10);
    m_ParametricSphere->setProperty("spherePhiRes", 10);
    m_ParametricSphere->setProperty("sphereRadius", 5);
    m_ParametricSphere->updateSurface();


    //Check if parameters had changed.
    QCOMPARE(m_ParametricSphere->property("sphereRadius").toDouble(), 5.0);
    QCOMPARE(m_ParametricSphere->property("sphereThetaRes").toDouble(), 10.0);
    QCOMPARE(m_ParametricSphere->property("spherePhiRes").toDouble(), 10.0);

    QCOMPARE(m_ParametricSphere->center()[0], 10.0);
    QCOMPARE(m_ParametricSphere->center()[1], 5.0);
    QCOMPARE(m_ParametricSphere->center()[2], 20.5);

    pipe->setGraphicObject(NULL);
    mafDEL(pipe);
}


MAF_REGISTER_TEST(mafVTKParametricSurfaceSphereTest);
#include "mafVTKParametricSurfaceSphereTest.moc"

