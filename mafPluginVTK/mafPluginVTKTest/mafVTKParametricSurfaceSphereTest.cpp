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
#include <mafVisualPipeVTKSurface.h>
#include <mafVTKParametricSurfaceSphere.h>

#include <mafVTKParametricSurfaceSphere.h>
#include <mafContainer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <QDebug>

#ifdef WIN32
    #define TEST_LIBRARY_NAME "mafPluginVTK_d.dll"
#else
    #ifdef __APPLE__
        #define TEST_LIBRARY_NAME "libmafPluginVTK_debug.dylib"
    #else
        #define TEST_LIBRARY_NAME "libmafPluginVTK_debug.so"
    #endif
#endif

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
//mafVTKParametricSurfaceSphere represent a VTK sphere surface with modificable parametrs.
//! </description>

class mafVTKParametricSurfaceSphereTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafVisualPipeVTKSurface);

        // Create the parametric sphere.
        m_ParametricSphere = mafNEW(mafPluginVTK::mafVTKParametricSurfaceSphere);

        //! <snippet>
        //// m_DataSourceContainer is the container of type vtkAlgorithmOutput
        //// to "wrap" the vtkPolyData just simply use the code below.
        m_DataSourceContainer = m_ParametricSphere->output();;

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
        mafDEL(m_ParametricSphere);
    }

    /// Test the creation of the vtkActor
    void createSphereTest();


private:
    mafVTKParametricSurfaceSphere *m_ParametricSphere; ///< Parametric sphere.
    mafVME *m_VME; ///< Contain the only item vtkPolydata representing a surface.
    mafResources::mafDataSet *m_DataSet;
    mafContainer<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
};

void mafVTKParametricSurfaceSphereTest::createSphereTest() {
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
    m_ParametricSphere->setCenter(newCenter);
    m_ParametricSphere->setProperty("sphereTheRes", 20);
    m_ParametricSphere->setProperty("spherePhiRes", 20);
    m_ParametricSphere->setProperty("sphereRadius", 5);


    //Check if parameters had changed.
    QCOMPARE(m_ParametricSphere->sphereRadius(), 5.0);
    QCOMPARE(m_ParametricSphere->spherePhiResolution(), 20.0);
    QCOMPARE(m_ParametricSphere->sphereThetaResolution(), 20.0);

    QCOMPARE(m_ParametricSphere->center()[0], 10.0);
    QCOMPARE(m_ParametricSphere->center()[1], 5.0);
    QCOMPARE(m_ParametricSphere->center()[2], 20.5);

    renWin->Delete();
    renderer->Delete();
    iren->Delete();
    mafDEL(pipe);
}


MAF_REGISTER_TEST(mafVTKParametricSurfaceSphereTest);
#include "mafVTKParametricSurfaceSphereTest.moc"

