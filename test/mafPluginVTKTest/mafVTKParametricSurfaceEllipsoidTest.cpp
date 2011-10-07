/*
 *  mafVTKParametricSurfaceEllipsoidTest.cpp
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
#include <mafVTKParametricSurfaceEllipsoid.h>
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
 Class name: mafVTKParametricSurfaceEllipsoidTest
 This class implements the test suite for mafVTKParametricSurfaceEllipsoid.
 */

//! <title>
//mafVTKParametricSurfaceEllipsoid
//! </title>
//! <description>
//mafVTKParametricSurfaceEllipsoid represent a VTK ellipsoid surface with modificable parameters.
//! </description>

class mafVTKParametricSurfaceEllipsoidTest : public QObject {
    Q_OBJECT
    //initialize all the graphic resources
    void initializeGraphicResources();

    //shutdown all the graphic resources
    void shutdownGraphicResources();

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKSurface);

        // Create the parametric ellipsoid.
        m_ParametricEllipsoid = mafNEW(mafPluginVTK::mafVTKParametricSurfaceEllipsoid);

        //! <snippet>
        //// m_DataSourceContainer is the container of type vtkAlgorithmOutput
        //// to "wrap" the vtkPolyData just simply use the code below.
        m_DataSourceContainer.setClassTypeNameFunction(vtkClassTypeNameExtract);
        m_DataSourceContainer = m_ParametricEllipsoid->output();

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
        mafDEL(m_ParametricEllipsoid);
        mafMessageHandler::instance()->shutdown();
        shutdownGraphicResources();
    }

    /// visualize parametric surface
    void visualizeTest();


private:
    mafVTKParametricSurfaceEllipsoid *m_ParametricEllipsoid; ///< Parametric ellipsoid.
    mafVME *m_VME; ///< Contain the only item vtkPolydata representing a surface.
    mafResources::mafDataSet *m_DataSet;
    mafProxy<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
    QObject *m_RenderWidget; /// renderer widget
    vtkRenderer *m_Renderer; ///< Accessory renderer
    QMainWindow *w;
};

void mafVTKParametricSurfaceEllipsoidTest::initializeGraphicResources() {
    w = new QMainWindow();
    w->setMinimumSize(640,480);

    m_RenderWidget = new mafVTKWidget();
    ((mafVTKWidget*)m_RenderWidget)->setParent(w);

    m_Renderer = ((mafVTKWidget*)m_RenderWidget)->renderer();

    m_Renderer->SetBackground(0.1, 0.1, 0.1);
    ((mafVTKWidget*)m_RenderWidget)->update();
    w->show();
}

void mafVTKParametricSurfaceEllipsoidTest::shutdownGraphicResources() {
    w->close();
}

void mafVTKParametricSurfaceEllipsoidTest::visualizeTest() {
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
    m_Renderer->ResetCamera();

    ((mafVTKWidget*)m_RenderWidget)->update();
    ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->Render();

    //iren->Start();
    QTest::qSleep(2000);

    double newCenter[3];
    newCenter[0] = 10;
    newCenter[1] = 5;
    newCenter[2] = 20.5;

    //Change parameters of the ellipsoid.
    m_ParametricEllipsoid->setCenter(newCenter);
    m_ParametricEllipsoid->setProperty("ellipsoidThetaRes", 10);
    m_ParametricEllipsoid->setProperty("ellipsoidPhiRes", 10);
    m_ParametricEllipsoid->setProperty("ellipsoidXLength", 5);
    m_ParametricEllipsoid->setProperty("ellipsoidYLength", 10);
    m_ParametricEllipsoid->setProperty("ellipsoidZLength", 15);
    m_ParametricEllipsoid->updateSurface();

    //Check if parameters had changed.
    QCOMPARE(m_ParametricEllipsoid->property("ellipsoidPhiRes").toDouble(), 10.0);
    QCOMPARE(m_ParametricEllipsoid->property("ellipsoidThetaRes").toDouble(), 10.0);
    QCOMPARE(m_ParametricEllipsoid->property("ellipsoidXLength").toDouble(), 5.0);
    QCOMPARE(m_ParametricEllipsoid->property("ellipsoidYLength").toDouble(), 10.0);
    QCOMPARE(m_ParametricEllipsoid->property("ellipsoidZLength").toDouble(), 15.0);

    QCOMPARE(m_ParametricEllipsoid->center()[0], 10.0);
    QCOMPARE(m_ParametricEllipsoid->center()[1], 5.0);
    QCOMPARE(m_ParametricEllipsoid->center()[2], 20.5);

    pipe->setGraphicObject(NULL);
    mafDEL(pipe);
}


MAF_REGISTER_TEST(mafVTKParametricSurfaceEllipsoidTest);
#include "mafVTKParametricSurfaceEllipsoidTest.moc"
