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

private slots:
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
        m_DataSourceContainer = m_ParametricEllipsoid->output();

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
        mafDEL(m_ParametricEllipsoid);
        mafMessageHandler::instance()->shutdown();
    }

   /// Test Set/Get method of parametric surface
    void SetGetTest();


private:
    mafVTKParametricSurfaceEllipsoid *m_ParametricEllipsoid; ///< Parametric ellipsoid.
    mafVME *m_VME; ///< Contain the only item vtkPolydata representing a surface.
    mafResources::mafDataSet *m_DataSet;
    mafContainer<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
};

void mafVTKParametricSurfaceEllipsoidTest::SetGetTest() {
    mafPipeVisualVTKSurface *pipe;
    pipe = mafNEW(mafPluginVTK::mafPipeVisualVTKSurface);
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

    //Change parameters of the ellipsoid.
    m_ParametricEllipsoid->setCenter(newCenter);
    m_ParametricEllipsoid->setProperty("ellipsoidTheRes", 10);
    m_ParametricEllipsoid->setProperty("ellipsoidPhiRes", 10);
    m_ParametricEllipsoid->setProperty("ellipsoidXLength", 5);
    m_ParametricEllipsoid->setProperty("ellipsoidYLength", 10);
    m_ParametricEllipsoid->setProperty("ellipsoidZLength", 15);
    m_ParametricEllipsoid->updateSurface();

    //Check if parameters had changed.
    QCOMPARE(m_ParametricEllipsoid->property("ellipsoidPhiRes").toDouble(), 10.0);
    QCOMPARE(m_ParametricEllipsoid->property("ellipsoidTheRes").toDouble(), 10.0);
    QCOMPARE(m_ParametricEllipsoid->property("ellipsoidXLength").toDouble(), 5.0);
    QCOMPARE(m_ParametricEllipsoid->property("ellipsoidYLength").toDouble(), 10.0);
    QCOMPARE(m_ParametricEllipsoid->property("ellipsoidZLength").toDouble(), 15.0);

    QCOMPARE(m_ParametricEllipsoid->center()[0], 10.0);
    QCOMPARE(m_ParametricEllipsoid->center()[1], 5.0);
    QCOMPARE(m_ParametricEllipsoid->center()[2], 20.5);

    renWin->Delete();
    renderer->Delete();
    iren->Delete();
    mafDEL(pipe);
}


MAF_REGISTER_TEST(mafVTKParametricSurfaceEllipsoidTest);
#include "mafVTKParametricSurfaceEllipsoidTest.moc"
