/*
 *  mafPipeVisualVTKMIPVolumeTest.cpp
 *  mafPluginVTK
 *
 *  Created by Daniele Giunchi on 24/06/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafEventBusManager.h>
#include <mafPipeVisualVTKMIPVolume.h>
#include <mafDataBoundaryAlgorithmVTK.h>
#include <mafResourcesRegistration.h>
#include <mafPipeVisual.h>
#include <mafProxy.h>
#include <mafPluginManager.h>
#include <mafPlugin.h>
#include <mafVME.h>
#include <mafDataSet.h>

#include <mafVTKWidget.h>

#include <vtkDataSetReader.h>
#include <vtkSmartPointer.h>

#include <vtkImageData.h>
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
 Class name: mafPipeVisualVTKMIPVolumeTest
 This class import a vtk structuredpoints mafPipeVisualVTKMIPVolume
 */

//! <title>
//mafPipeVisualVTKMIPVolume
//! </title>
//! <description>
//mafPipeVisualVTKMIPVolume is a visual pipe used to render VTK volume.
//It takes a mafDataSet as input and returns a mafProxy<vtkActor>.
//! </description>

class mafPipeVisualVTKMIPVolumeTest : public QObject {
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
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKMIPVolume)

        m_VTKFile = "/Users/danielegiunchi/Desktop/sample/CropTestVolumeSP.vtk";
        
        // Import a vtk volume.
        m_Reader = vtkDataSetReader::New();
        QByteArray ba = m_VTKFile.toAscii();
        m_Reader->SetFileName(ba.constData());
        m_Reader->Update();
        
        //! <snippet>
        //// Create a container with the outputPort of a vtkCubeSource
        //// m_DataSourceContainer is the container of type vtkAlgorithmOutput
        //// to "wrap" the 'vtkCubeSource' of type vtkPolyData just simply use the code below.
        m_DataSourceContainer.setClassTypeNameFunction(vtkClassTypeNameExtract);
        m_DataSourceContainer = m_Reader->GetOutputPort(0);

        //Insert data into VME
        m_VME = mafNEW(mafResources::mafVME);
        m_DataSet = mafNEW(mafResources::mafDataSet);
        mafDataBoundaryAlgorithmVTK *boundaryAlgorithm;
        boundaryAlgorithm = mafNEW(mafDataBoundaryAlgorithmVTK);
        m_DataSet->setBoundaryAlgorithm(boundaryAlgorithm);
        m_DataSet->setDataValue(&m_DataSourceContainer);
        m_VME->dataSetCollection()->insertItem(m_DataSet, 0);
        //! </snippet>
        m_RenderWidget = new mafVTKWidget();
        initializeGraphicResources();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_DataSet);
        //here delete vtk stuff
        m_Reader->Delete();
        mafDEL(m_VME);
        mafMessageHandler::instance()->shutdown();
        delete m_RenderWidget;
        shutdownGraphicResources();
    }

    /// Test the creation of the vtkActor
    void updatePipeTest();

    /// Test the creation of the vtkActor
    void updatePipeTestFromPlugIn();

private:
    mafVME *m_VME; ///< Contain the only item vtkPolydata representing a surface.
    mafResources::mafDataSet *m_DataSet;
    mafProxy<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
    vtkDataSetReader *m_DataSetReader; /// reader used for import
    vtkDataSetReader *m_Reader;
    QString m_VTKFile; ///< file name of the vtk test
    QObject *m_RenderWidget; /// renderer widget
    vtkRenderer *m_Renderer; ///< Accessory renderer
    QMainWindow *w;
};

void mafPipeVisualVTKMIPVolumeTest::initializeGraphicResources() {
    w = new QMainWindow();
    w->setMinimumSize(640,480);

    m_RenderWidget = new mafVTKWidget();
    ((mafVTKWidget*)m_RenderWidget)->setParent(w);

    m_Renderer = ((mafVTKWidget*)m_RenderWidget)->renderer();

    m_Renderer->SetBackground(0.1, 0.1, 0.1);
    ((mafVTKWidget*)m_RenderWidget)->update();
    w->show();
}

void mafPipeVisualVTKMIPVolumeTest::shutdownGraphicResources() {
    w->close();
}

void mafPipeVisualVTKMIPVolumeTest::updatePipeTest() {
    mafPipeVisualVTKMIPVolume *pipe;
    pipe = mafNEW(mafPluginVTK::mafPipeVisualVTKMIPVolume);
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
    
    QTest::qSleep(2000);
    pipe->setGraphicObject(NULL);
    mafDEL(pipe);
}

void mafPipeVisualVTKMIPVolumeTest::updatePipeTestFromPlugIn() {
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

    QString check("mafPluginVTK::mafPipeVisualVTKMIPVolume");
    QVERIFY(binding_class_list.contains(check));

    //! <snippet>
    mafPipeVisual *visualPipe = (mafPipeVisual *)mafNEWFromString("mafPluginVTK::mafPipeVisualVTKMIPVolume");
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
    //iren->Start();
    QTest::qSleep(2000);

    //Change scalarVisibility flag
    mafDEL(visualPipe);

    pluginManager->shutdown();
}

//MAF_REGISTER_TEST(mafPipeVisualVTKMIPVolumeTest);
#include "mafPipeVisualVTKMIPVolumeTest.moc"
