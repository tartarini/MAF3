/*
 *  mafVisualPipeVTKSurfaceTest.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 01/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafEventBusManager.h>
#include <mafVisualPipeVTKSurface.h>
#include <mafResourcesRegistration.h>
#include <mafVisualPipe.h>
#include <mafContainer.h>
#include <mafPluginManager.h>
#include <mafPlugin.h>
#include <vtkAppendPolyData.h>
#include <vtkCubeSource.h>
#include <vtkSmartPointer.h>

#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkSphereSource.h>
#include <vtkAlgorithmOutput.h>

// render window stuff
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#ifdef WIN32
    #define TEST_LIBRARY_NAME "mafPluginVTK_d.dll"
#else
    #ifdef __APPLE__
        #define TEST_LIBRARY_NAME "libmafPluginVTK_debug.dylib"
    #else
        #define TEST_LIBRARY_NAME "libmafPluginVTK_debug.so"
    #endif
#endif

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;


/**
 Class name: mafVisualPipeVTKSurfaceTest
 This class creates a vtkSphereSource and visualizes it trough the mafVisualPipeVTKSurface
 */

//! <title>
//mafVisualPipeVTKSurface
//! </title>
//! <description>
//mafVisualPipeVTKSurface is a visual pipe used to render VTK polydata.
//It takes a mafDataSet as input and returns a mafContainer<vtkActor>.
//! </description>

class mafVisualPipeVTKSurfaceTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafVisualPipeVTKSurface);

        // Create a sphere.
        vtkSmartPointer<vtkSphereSource> surfSphere = vtkSmartPointer<vtkSphereSource>::New();
        surfSphere->SetRadius(5);
        surfSphere->SetPhiResolution(10);
        surfSphere->SetThetaResolution(10);
        surfSphere->Update();

        // Create a cube.
        vtkSmartPointer<vtkCubeSource> surfCube = vtkSmartPointer<vtkCubeSource>::New();
        surfCube->SetXLength(3);
        surfCube->SetYLength(6);
        surfCube->SetZLength(4);
        surfCube->SetCenter(10,5,10);
        surfCube->Update();

        // Append the 2 surfaces in one polydata
        m_AppendData = vtkAppendPolyData::New();
        m_AppendData->AddInputConnection(surfSphere->GetOutputPort());
        m_AppendData->AddInputConnection(surfCube->GetOutputPort());

        //! <snippet>
        //// Create a container with the outputPort of a vtkCubeSource
        //// m_DataSourceContainer is the container of type vtkAlgorithmOutput
        //// to "wrap" the 'vtkCubeSource' of type vtkPolyData just simply use the code below.
        m_DataSourceContainer = m_AppendData->GetOutputPort(0);;

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
        m_AppendData->Delete();
        mafDEL(m_VME);
        mafMessageHandler::instance()->shutdown();
    }

    /// Test the creation of the vtkActor
    void updatePipeTest();

    /// Test the creation of the vtkActor
    void updatePipeTestFromPlugIn();

private:
    mafVME *m_VME; ///< Contain the only item vtkPolydata representing a surface.
    mafResources::mafDataSet *m_DataSet;
    mafContainer<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
    vtkAppendPolyData *m_AppendData; /// Bunch of surfaces.
};

void mafVisualPipeVTKSurfaceTest::updatePipeTest() {
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

    //Change scalarVisibility flag
    pipe->setProperty("scalarVisibility", 1);
    pipe->updatePipe();
    renWin->Render();
    QTest::qSleep(2000);

    renWin->Delete();
    renderer->Delete();
    iren->Delete();
    mafDEL(pipe);
}

void mafVisualPipeVTKSurfaceTest::updatePipeTestFromPlugIn() {
    mafPluginManager *pluginManager = mafPluginManager::instance();
    mafString pluginName = TEST_LIBRARY_NAME;

    // Load the library containing the visual pipe that I want to plug-in.
    pluginManager->loadPlugin(pluginName);

    // Dump the plug-in information.
    mafPluginInfo info = pluginManager->pluginInformation(pluginName);
    mafMsgDebug() << "Plug-in Information:";
    mafMsgDebug() << "Varsion: " << info.m_Version;
    mafMsgDebug() << "Author: " << info.m_Author;
    mafMsgDebug() << "Vendor: " << info.m_Vendor;
    mafMsgDebug() << "Description: " << info.m_Description;

    mafStringList binding_class_list;
    binding_class_list = mafCoreRegistration::acceptObject(m_VME);
    int num = binding_class_list.count();
    QVERIFY(num == 2);

    mafString visualPipeType = binding_class_list.at(1);
    QCOMPARE(visualPipeType,mafString("mafPluginVTK::mafVisualPipeVTKSurface"));

    //! <snippet>
    mafVisualPipe *visualPipe = (mafVisualPipe *)mafNEWFromString(visualPipeType);
    visualPipe->setProperty("scalarVisibility", 1);
    visualPipe->setInput(m_VME);
    visualPipe->createPipe();
    visualPipe->updatePipe();
    //! </snippet>

    //! <snippet>
    mafContainer<vtkActor> *actor = mafContainerPointerTypeCast(vtkActor, visualPipe->output());
    //! </snippet>
    vtkRenderWindow *renWin = vtkRenderWindow::New();
    vtkRenderer *renderer = vtkRenderer::New();
    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();

    renderer->AddActor(*actor);
    renWin->AddRenderer(renderer);
    iren->SetRenderWindow(renWin);

    renderer->SetBackground(0.1, 0.1, 0.1);
    renWin->SetSize(640, 480);
    renWin->SetPosition(400,0);

    renWin->Render();
    //iren->Start();
    QTest::qSleep(2000);

    //Change scalarVisibility flag
    visualPipe->setProperty("scalarVisibility", 0);
    visualPipe->updatePipe();
    renWin->Render();
    QTest::qSleep(2000);
    renWin->Delete();
    renderer->Delete();
    iren->Delete();
    mafDEL(visualPipe);

    pluginManager->shutdown();
}

MAF_REGISTER_TEST(mafVisualPipeVTKSurfaceTest);
#include "mafVisualPipeVTKSurfaceTest.moc"
