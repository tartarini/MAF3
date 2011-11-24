/*
 *  mafPipeVisualVTKSliceSurfaceTest.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 12/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafEventBusManager.h>
#include <mafPipeVisualVTKSliceSurface.h>
#include <mafDataBoundaryAlgorithmVTK.h>
#include <mafResourcesRegistration.h>
#include <mafPipeVisual.h>
#include <mafProxy.h>
#include <mafPluginManager.h>
#include <mafPlugin.h>
#include <mafVME.h>
#include <mafDataSet.h>

#include <mafVTKWidget.h>

#include <vtkSmartPointer.h>

#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkSphereSource.h>
#include <vtkAlgorithmOutput.h>

// render window stuff
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <QMainWindow>

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
 Class name: mafPipeVisualVTKSliceSurfaceTest
 This class creates a vtkSphereSource and visualizes it trough the mafPipeVisualVTKSliceSurface
 */

//! <title>
//mafPipeVisualVTKSliceSurface
//! </title>
//! <description>
//mafPipeVisualVTKSliceSurface is a visual pipe used to render VTK PolyData sliced through a plain.
//It takes a mafDataSet as input and returns a mafProxy<vtkActor>.
//! </description>

class mafPipeVisualVTKSliceSurfaceTest : public QObject {
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
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKSliceSurface)

        // Create a sphere.
        m_SurfSphere = vtkSphereSource::New();
        m_SurfSphere->SetRadius(5);
        m_SurfSphere->SetPhiResolution(10);
        m_SurfSphere->SetThetaResolution(10);
        m_SurfSphere->Update();

        //! <snippet>
        //// Create a container with the outputPort of a vtkCubeSource
        //// m_DataSourceContainer is the container of type vtkAlgorithmOutput
        //// to "wrap" the 'vtkCubeSource' of type vtkPolyData just simply use the code below.
        m_DataSourceContainer.setClassTypeNameFunction(vtkClassTypeNameExtract);
        m_DataSourceContainer = m_SurfSphere->GetOutputPort(0);

        //Insert data into VME
        m_VME = mafNEW(mafResources::mafVME);
        mafResources::mafDataSet *dataSet = mafNEW(mafResources::mafDataSet);
        mafDataBoundaryAlgorithmVTK *boundaryAlgorithm;
        boundaryAlgorithm = mafNEW(mafDataBoundaryAlgorithmVTK);
        dataSet->setBoundaryAlgorithm(boundaryAlgorithm);
        dataSet->setDataValue(&m_DataSourceContainer);
        m_VME->dataSetCollection()->insertItem(dataSet, 0);
        mafDEL(dataSet);
        //! </snippet>
        m_RenderWidget = new mafVTKWidget();
        initializeGraphicResources();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_VME);
        mafMessageHandler::instance()->shutdown();
        m_RenderWidget = new mafVTKWidget();
        shutdownGraphicResources();

    }

    /// Test the creation of the vtkActor
    void updatePipeTest();

    /// Test the creation of the vtkActor
    void updatePipeTestFromPlugIn();

private:
    mafVME *m_VME; ///< Contain the only item vtkPolydata representing a surface.
    mafProxy<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
    vtkSphereSource *m_SurfSphere;
    QObject *m_RenderWidget; /// renderer widget
    vtkRenderer *m_Renderer; ///< Accessory renderer
    QMainWindow *w;
};

void mafPipeVisualVTKSliceSurfaceTest::initializeGraphicResources() {
    w = new QMainWindow();
    w->setMinimumSize(640,480);
    w->setWindowTitle("mafPipeVisualVTKSliceSurface Test");

    m_RenderWidget = new mafVTKWidget();
    ((mafVTKWidget*)m_RenderWidget)->setParent(w);

    m_Renderer = ((mafVTKWidget*)m_RenderWidget)->renderer();

    m_Renderer->SetBackground(0.1, 0.1, 0.1);
    ((mafVTKWidget*)m_RenderWidget)->update();
    w->show();
}

void mafPipeVisualVTKSliceSurfaceTest::shutdownGraphicResources() {
    w->close();
}

void mafPipeVisualVTKSliceSurfaceTest::updatePipeTest() {
    mafPipeVisualVTKSliceSurface *pipe;
    pipe = mafNEW(mafPluginVTK::mafPipeVisualVTKSliceSurface);
    pipe->setInput(m_VME);
    pipe->setProperty("originX", 0.);
    pipe->setProperty("originY", 0.);
    pipe->setProperty("originZ", 2.);
    pipe->setProperty("normalX", 0.);
    pipe->setProperty("normalY", 0.);
    pipe->setProperty("normalZ", 1.);
    pipe->setGraphicObject(m_RenderWidget);
    pipe->updatePipe();

    // Get the vtkActor from the visual pipe
    // And assign to a mafProxy
    mafProxy<vtkActor> *actor = mafProxyPointerTypeCast(vtkActor, pipe->output());
    QVERIFY(actor != NULL);

    // Connect the actor (contained into the container) with the renderer.
    m_Renderer->AddActor(*actor);

    ((mafVTKWidget*)m_RenderWidget)->update();
    m_Renderer->ResetCamera();
    ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->Render();
    QTest::qSleep(2000);

    //Change slice's normal & origin
    pipe->setProperty("normalX", 0.);
    pipe->setProperty("normalY", 1.);
    pipe->setProperty("normalZ", 0.);
    pipe->setProperty("originX", 0.);
    pipe->setProperty("originY", 0.);
    pipe->setProperty("originZ", 0.);
    pipe->updatePipe();
    QTest::qSleep(2000);

    pipe->setGraphicObject(NULL);
    mafDEL(pipe);
}

void mafPipeVisualVTKSliceSurfaceTest::updatePipeTestFromPlugIn() {
    mafPluginManager *pluginManager = mafPluginManager::instance();
    QString pluginName = TEST_LIBRARY_NAME;

    // Load the library containing the visual pipe that I want to plug-in.
    pluginManager->loadPlugin(pluginName);

    // Dump the plug-in information.
    mafPluginInfo info = pluginManager->pluginInformation(pluginName);
    qDebug() << "Plug-in Information:";
    qDebug() << "Version: " << info.m_Version;
    qDebug() << "Author: " << info.m_Author;
    qDebug() << "Vendor: " << info.m_Vendor;
    qDebug() << "Description: " << info.m_Description;

    QStringList binding_class_list;
    binding_class_list = mafCoreRegistration::acceptObject(m_VME);
    int num = binding_class_list.count();
    QVERIFY(num != 0);
    
    QString check("mafPluginVTK::mafPipeVisualVTKSliceSurface");
    QVERIFY(binding_class_list.contains(check));
    
    //! <snippet>
    mafPipeVisual *visualPipe = (mafPipeVisual *)mafNEWFromString("mafPluginVTK::mafPipeVisualVTKSliceSurface");
    visualPipe->setProperty("originX", 0.);
    visualPipe->setProperty("originY", 0.);
    visualPipe->setProperty("originZ", 2.);
    visualPipe->setProperty("normalX", 0.);
    visualPipe->setProperty("normalY", 0.);
    visualPipe->setProperty("normalZ", 1.);
    visualPipe->setInput(m_VME);
    visualPipe->setGraphicObject(m_RenderWidget);
    visualPipe->updatePipe();
    //! </snippet>

    //! <snippet>
    mafProxy<vtkActor> *actor = mafProxyPointerTypeCast(vtkActor, visualPipe->output());
    //! </snippet>

    m_Renderer->AddActor(*actor);
    ((mafVTKWidget*)m_RenderWidget)->update();
    m_Renderer->ResetCamera();
    ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->Render();

    QTest::qSleep(2000);

    //Change slice's normal & origin
    visualPipe->setProperty("normalX", 0.);
    visualPipe->setProperty("normalY", 1.);
    visualPipe->setProperty("normalZ", 0.);
    visualPipe->setProperty("originX", 0.);
    visualPipe->setProperty("originY", 3.);
    visualPipe->setProperty("originZ", 0.);
    visualPipe->updatePipe();
    ((mafVTKWidget*)m_RenderWidget)->update();
    m_Renderer->ResetCamera();
    ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->Render();
    QTest::qSleep(2000);
    mafDEL(visualPipe);
    pluginManager->shutdown();
}

MAF_REGISTER_TEST(mafPipeVisualVTKSliceSurfaceTest);
#include "mafPipeVisualVTKSliceSurfaceTest.moc"
