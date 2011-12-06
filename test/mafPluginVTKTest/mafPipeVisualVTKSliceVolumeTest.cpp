/*
 *  mafPipeVisualVTKSliceVolumeTest.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 12/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestConfig.h>
#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafEventBusManager.h>
#include <mafPipeVisualVTKSliceVolume.h>
#include <mafDataBoundaryAlgorithmVTK.h>
#include <mafResourcesRegistration.h>
#include <mafPipeVisual.h>
#include <mafProxy.h>
#include <mafPluginManager.h>
#include <mafPlugin.h>
#include <mafVME.h>
#include <mafDataSetCollection.h>
#include <mafDataSet.h>

#include <mafVTKWidget.h>

#include <vtkSmartPointer.h>

#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkDataSetReader.h>
#include <vtkAlgorithmOutput.h>

#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>

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
 Class name: mafPipeVisualVTKSliceVolumeTest
 This class creates a vtkSphereSource and visualizes it trough the mafPipeVisualVTKSliceVolume
 */

//! <title>
//mafPipeVisualVTKSliceVolume
//! </title>
//! <description>
//mafPipeVisualVTKSliceVolume is a visual pipe used to render VTK Volume sliced through a plain.
//It takes a mafDataSet as input and returns a mafProxy<vtkActor>.
//! </description>

class mafPipeVisualVTKSliceVolumeTest : public QObject {
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
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKSliceVolume)

        QString fname(MAF_DATA_DIR);
        fname.append("/VTK/mafPipeVisualVTKSliceVolumeTestData.vtk");
        
        // Import a VTK volume.
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
        mafDEL(m_VME);
        mafMessageHandler::instance()->shutdown();

        shutdownGraphicResources();
    }

    /// Test the creation of the vtkActor
    void updatePipeTest();

    /// Test the creation of the vtkActor
    void updatePipeTestFromPlugIn();

private:
    mafVME *m_VME; ///< Contain the only item vtkPolydata representing a Volume.
    mafProxy<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
    vtkDataSetReader *m_Reader;

    QObject *m_RenderWidget; /// renderer widget
    vtkRenderer *m_Renderer; ///< Accessory renderer
    QMainWindow *w;
};

void mafPipeVisualVTKSliceVolumeTest::initializeGraphicResources() {
    w = new QMainWindow();
    w->setMinimumSize(640,480);
    w->setWindowTitle("mafPipeVisualVTKSliceVolume Test");

    m_RenderWidget = new mafVTKWidget();
    ((mafVTKWidget*)m_RenderWidget)->setParent(w);

    m_Renderer = ((mafVTKWidget*)m_RenderWidget)->renderer();

    m_Renderer->SetBackground(0.1, 0.1, 0.1);
    ((mafVTKWidget*)m_RenderWidget)->update();
    w->show();
}

void mafPipeVisualVTKSliceVolumeTest::shutdownGraphicResources() {
    w->close();
}

void mafPipeVisualVTKSliceVolumeTest::updatePipeTest() {
    mafPipeVisualVTKSliceVolume *pipe;
    pipe = mafNEW(mafPluginVTK::mafPipeVisualVTKSliceVolume);
    pipe->setInput(m_VME);
    QVariantList b = m_VME->dataSetCollection()->itemAtCurrentTime()->bounds();
    double center[3];
    center[0] = (b[0].toDouble() + b[1].toDouble()) / 2.;
    center[1] = (b[2].toDouble() + b[3].toDouble()) / 2.;
    center[2] = (b[4].toDouble() + b[5].toDouble()) / 2.;
    pipe->setProperty("originX", center[0]);
    pipe->setProperty("originY", center[1]);
    pipe->setProperty("originZ", center[2]);
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
    
    vtkOutlineFilter *m_OutlineFilter = vtkOutlineFilter::New();
    m_OutlineFilter->SetInputConnection(0, m_Reader->GetOutputPort(0));
    vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(0, m_OutlineFilter->GetOutputPort());
    vtkActor *actorOutline = vtkActor::New();
    actorOutline->SetMapper(mapper);
    mapper->Delete();
    m_Renderer->GetActiveCamera()->SetRoll(20);
    m_Renderer->GetActiveCamera()->Azimuth(20);
    m_Renderer->GetActiveCamera()->Yaw(20);
    m_Renderer->AddActor(actorOutline);

    ((mafVTKWidget*)m_RenderWidget)->update();
    m_Renderer->ResetCamera();
    ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->Render();
        
    QTest::qSleep(12000);

    pipe->setGraphicObject(NULL);
    mafDEL(pipe);
}

void mafPipeVisualVTKSliceVolumeTest::updatePipeTestFromPlugIn() {
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
    
    QString check("mafPluginVTK::mafPipeVisualVTKSliceVolume");
    QVERIFY(binding_class_list.contains(check));
    
    //! <snippet>
    mafPipeVisual *visualPipe = (mafPipeVisual *)mafNEWFromString("mafPluginVTK::mafPipeVisualVTKSliceVolume");
    QVariantList b = m_VME->dataSetCollection()->itemAtCurrentTime()->bounds();
    double center[3];
    center[0] = (b[0].toDouble() + b[1].toDouble()) / 2.;
    center[1] = (b[2].toDouble() + b[3].toDouble()) / 2.;
    center[2] = (b[4].toDouble() + b[5].toDouble()) / 2.;
    visualPipe->setProperty("originX", center[0]);
    visualPipe->setProperty("originY", center[1]);
    visualPipe->setProperty("originZ", center[2]);
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

    mafDEL(visualPipe);
    pluginManager->shutdown();
}

MAF_REGISTER_TEST(mafPipeVisualVTKSliceVolumeTest);
#include "mafPipeVisualVTKSliceVolumeTest.moc"
