/*
 *  mafPipeVisualVTKSurfaceTest.cpp
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
#include <mafPipeVisualVTKSurface.h>
#include <mafDataBoundaryAlgorithmVTK.h>
#include <mafResourcesRegistration.h>
#include <mafPipeVisual.h>
#include <mafProxy.h>
#include <mafPluginManager.h>
#include <mafPlugin.h>

#include <mafVTKWidget.h>

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
    #define TEST_LIBRARY_NAME "mafPluginVTK.dll"
#else
  #ifdef __APPLE__
    #define TEST_LIBRARY_NAME "mafPluginVTK.dylib"
  #else
    #define TEST_LIBRARY_NAME "mafPluginVTK.so"
  #endif
#endif

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;


/**
 Class name: mafPipeVisualVTKSurfaceTest
 This class creates a vtkSphereSource and visualizes it trough the mafPipeVisualVTKSurface
 */

//! <title>
//mafPipeVisualVTKSurface
//! </title>
//! <description>
//mafPipeVisualVTKSurface is a visual pipe used to render VTK polydata.
//It takes a mafDataSet as input and returns a mafProxy<vtkActor>.
//! </description>

class mafPipeVisualVTKSurfaceTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKSurface)

        m_RenderWidget = new mafVTKWidget();
        
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
        m_DataSourceContainer.setClassTypeNameFunction(vtkClassTypeNameExtract);
        m_DataSourceContainer = m_AppendData->GetOutputPort(0);

        //Insert data into VME
        m_VME = mafNEW(mafResources::mafVME);
        m_DataSet = mafNEW(mafResources::mafDataSet);
        mafDataBoundaryAlgorithmVTK *boundaryAlgorithm;
        boundaryAlgorithm = mafNEW(mafDataBoundaryAlgorithmVTK);
        m_DataSet->setBoundaryAlgorithm(boundaryAlgorithm);
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
        delete m_RenderWidget;

    }

    /// Test the creation of the vtkActor
    void updatePipeTest();

    /// Test the creation of the vtkActor
    void updatePipeTestFromPlugIn();

private:
    mafVME *m_VME; ///< Contain the only item vtkPolydata representing a surface.
    mafResources::mafDataSet *m_DataSet;
    mafProxy<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
    vtkAppendPolyData *m_AppendData; /// Bunch of surfaces.
    
    QObject *m_RenderWidget; /// renderer widget
};

void mafPipeVisualVTKSurfaceTest::updatePipeTest() {
    vtkRenderer *renderer = vtkRenderer::New();
    vtkRenderWindow *renWin = ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow();

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

    renWin->AddRenderer(renderer);

    
    // Connect the actor (contained into the container) with the renderer.
    renderer->AddActor(*actor);

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

    renderer->Delete();
    pipe->setGraphicObject(NULL);
    mafDEL(pipe);
}

void mafPipeVisualVTKSurfaceTest::updatePipeTestFromPlugIn() {
    vtkRenderer *renderer = vtkRenderer::New();
    vtkRenderWindow *renWin = ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow();

    
    mafPluginManager *pluginManager = mafPluginManager::instance();
    QString pluginName = TEST_LIBRARY_NAME;

    // Load the library containing the visual pipe that I want to plug-in.
    pluginManager->loadPlugin(pluginName);

    // Dump the plug-in information.
    mafPluginInfo info = pluginManager->pluginInformation(pluginName);
    qDebug() << "Plug-in Information:";
    qDebug() << "Varsion: " << info.m_Version;
    qDebug() << "Author: " << info.m_Author;
    qDebug() << "Vendor: " << info.m_Vendor;
    qDebug() << "Description: " << info.m_Description;

    QStringList binding_class_list;
    binding_class_list = mafCoreRegistration::acceptObject(m_VME);
    int num = binding_class_list.count();
    QVERIFY(num != 0);
    
    QString check("mafPluginVTK::mafPipeVisualVTKSurface");
    QVERIFY(binding_class_list.contains(check));
    
    //! <snippet>
    mafPipeVisual *visualPipe = (mafPipeVisual *)mafNEWFromString("mafPluginVTK::mafPipeVisualVTKSurface");
    visualPipe->setProperty("scalarVisibility", 1);
    visualPipe->setInput(m_VME);
    visualPipe->setGraphicObject(m_RenderWidget);
    visualPipe->updatePipe();
    //! </snippet>

    //! <snippet>
    mafProxy<vtkActor> *actor = mafProxyPointerTypeCast(vtkActor, visualPipe->output());
    //! </snippet>

    renderer->AddActor(*actor);
    renWin->AddRenderer(renderer);

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
    renderer->Delete();
    mafDEL(visualPipe);

    pluginManager->shutdown();
}

MAF_REGISTER_TEST(mafPipeVisualVTKSurfaceTest);
#include "mafPipeVisualVTKSurfaceTest.moc"
