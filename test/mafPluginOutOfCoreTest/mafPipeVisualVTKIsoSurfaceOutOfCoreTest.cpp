/*
 *  mafPipeVisualVTKIsoSurfaceOutOfCoreTest.cpp
 *  mafPluginOutOfCore
 *
 *  Created by Yubo Tao on 18/06/12.
 *  Copyright 2012 University of Bedfordshire. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTestConfig.h"
#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafEventBusManager.h>
#include <mafPipeVisualVTKIsoSurfaceOutOfCore.h>
#include <mafDataBoundaryAlgorithmVTK.h>
#include <mafResourcesRegistration.h>
#include <mafPipeVisual.h>
#include <mafProxy.h>
#include <mafPluginManager.h>
#include <mafPlugin.h>
#include <mafVME.h>
#include <mafDataSet.h>
#include <mafVolume.h>
#include <mafMementoVolume.h>

#include <mafVTKWidget.h>

#include <vtkDataSetReader.h>
#include <vtkSmartPointer.h>

#include <vtkImageData.h>
#include <vtkActor.h>
#include <vtkAlgorithmOutput.h>

// render window stuff
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <QMainWindow>

#ifdef WIN32
    #define TEST_LIBRARY_NAME "mafPluginOutOfCore.dll"
#else
  #ifdef __APPLE__
    #define TEST_LIBRARY_NAME "mafPluginOutOfCore.dylib"
  #else
    #define TEST_LIBRARY_NAME "mafPluginOutOfCore.so"
  #endif
#endif

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;
using namespace mafPluginOutOfCore;


/**
 Class name: mafPipeVisualVTKIsoSurfaceOutOfCoreTest
 This class import a mafVolume and render it throught a mafPipeVisualVTKIsoSurfaceOutOfCore.
 */

//! <title>
//mafPipeVisualVTKIsoSurfaceOutOfCore
//! </title>
//! <description>
//This visual pipe allow to extract a surface from a out-of-core volume data 
//given a threshold value. The value is extracted according to the scalar values 
//present into the volume data. The isosurface is extracted in real time*/
//! </description>

class mafPipeVisualVTKIsoSurfaceOutOfCoreTest : public QObject {
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
        mafRegisterObjectAndAcceptBind(mafPluginOutOfCore::mafPipeVisualVTKIsoSurfaceOutOfCore)
        
        QString fname(MAF_DATA_DIR);
        fname.append("/VTK/mafPipeVisualVTKIsoSurfaceTestData.vtk");
        
        // Import a vtk volume.
        m_Reader = vtkDataSetReader::New();
        fname = QDir::toNativeSeparators(fname);
        QByteArray ba = fname.toAscii();
        m_Reader->SetFileName(ba.data());
        m_Reader->Update();
        
        //! <snippet>
        //// Create a container with the outputPort of a vtkCubeSource
        //// m_DataSourceContainer is the container of type vtkAlgorithmOutput
        //// to "wrap" the 'vtkCubeSource' of type vtkPolyData just simply use the code below.
        m_DataSourceContainer.setClassTypeNameFunction(vtkClassTypeNameExtract);
        m_DataSourceContainer = m_Reader->GetOutputPort(0);

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
        initializeGraphicResources();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        //here delete vtk stuff
        mafDEL(m_VME);
        mafMessageHandler::instance()->shutdown();
        shutdownGraphicResources();
    }

    /// Test the creation of the vtkActor
    void updatePipeTest();

    /// Test the creation of the vtkActor
    void updatePipeTestFromPlugIn();

private:
    mafVME *m_VME; ///< Contain the only item vtkPolydata representing a surface.
    mafProxy<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
    vtkDataSetReader *m_DataSetReader; /// reader used for import
    vtkDataSetReader *m_Reader;
    QString m_VTKFile; ///< file name of the vtk test
    QObject *m_RenderWidget; /// renderer widget
    vtkRenderer *m_Renderer; ///< Accessory renderer
    QMainWindow *w;
};

void mafPipeVisualVTKIsoSurfaceOutOfCoreTest::initializeGraphicResources() {
    w = new QMainWindow();
    w->setMinimumSize(640,480);
    w->setWindowTitle("mafPipeVisualVTKIsoSurfaceOutOfCore Test");

    m_RenderWidget = new mafVTKWidget();
    ((mafVTKWidget*)m_RenderWidget)->setParent(w);

    m_Renderer = ((mafVTKWidget*)m_RenderWidget)->renderer();

    m_Renderer->SetBackground(0.1, 0.1, 0.1);
    ((mafVTKWidget*)m_RenderWidget)->update();
    w->show();
}

void mafPipeVisualVTKIsoSurfaceOutOfCoreTest::shutdownGraphicResources() {
    w->close();
}

void mafPipeVisualVTKIsoSurfaceOutOfCoreTest::updatePipeTest() {
    mafPipeVisualVTKIsoSurfaceOutOfCore *pipe;
    pipe = mafNEW(mafPluginOutOfCore::mafPipeVisualVTKIsoSurfaceOutOfCore);
    pipe->setInput(m_VME);
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
    //ien->Start();
    QTest::qSleep(2000);

    pipe->setGraphicObject(NULL);
    mafDEL(pipe);
}

void mafPipeVisualVTKIsoSurfaceOutOfCoreTest::updatePipeTestFromPlugIn() {
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

    QString check("mafPluginVTK::mafPipeVisualVTKIsoSurfaceOutOfCore");
    QVERIFY(binding_class_list.contains(check));


    //! <snippet>
    mafPipeVisual *visualPipe = (mafPipeVisual *)mafNEWFromString("mafPluginOutOfCore::mafPipeVisualVTKIsoSurfaceOutOfCore");
    visualPipe->setInput(m_VME);
    visualPipe->setGraphicObject(m_RenderWidget);
    visualPipe->updatePipe();
    //! </snippet>

    //! <snippet>
    mafProxy<vtkActor> *actor = mafProxyPointerTypeCast(vtkActor, visualPipe->output());
    //! </snippet>
    
    m_Renderer->AddActor(*actor);
    m_Renderer->ResetCamera();
    
    ((mafVTKWidget*)m_RenderWidget)->update();
    ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->Render();
    //m_Iren->Start();
    QTest::qSleep(2000);

    mafDEL(visualPipe);
    pluginManager->shutdown();
}

MAF_REGISTER_TEST(mafPipeVisualVTKIsoSurfaceOutOfCoreTest);
#include "mafPipeVisualVTKIsoSurfaceOutOfCoreTest.moc"
