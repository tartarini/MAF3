/*
 *  mafPipeVisualVTKIsoSurfaceOutOfCoreTest.cpp
 *  mafPluginVTK
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
#include <mafResourcesRegistration.h>
#include <mafPipeVisual.h>
#include <mafProxy.h>
#include <mafPluginManager.h>
#include <mafPlugin.h>
#include <mafVME.h>
#include <mafDataSet.h>
#include <mafVolume.h>
#include <mafMementoVolume.h>
#include <mafExternalDataCodecVolume.h>

// render window stuff
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <mafVTKWidget.h>
#include <QMainWindow>

#ifdef WIN32
    #define SERIALIZATION_LIBRARY_NAME "mafSerialization.dll"
#else
    #ifdef __APPLE__
        #define SERIALIZATION_LIBRARY_NAME "mafSerialization.dylib"
    #else
        #define SERIALIZATION_LIBRARY_NAME "mafSerialization.so"
    #endif
#endif

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

    //encode the volume for testing
    void encodeVolume();

    //decode the volume from the file for testing
    void decodeVolume();

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// Test the creation of the vtkActor
    void updatePipeTest();

    /// Test the creation of the vtkActor
    void updatePipeTestFromPlugIn();

private:
    mafProxy<mafVolume> m_Volume;       ///< the decoded volume from m_VolumeFileName.
    mafVME *m_VME;                      ///< Contain the only item vtkPolydata representing a surface.
    QString m_VolumeFileName;           ///< the volume file name.

    QObject *m_RenderWidget;            /// renderer widget
    vtkRenderer *m_Renderer;            ///< Accessory renderer
    QMainWindow *w;

    QLibrary *m_SerializationHandle;
};

void mafPipeVisualVTKIsoSurfaceOutOfCoreTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    mafResourcesRegistration::registerResourcesObjects();
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKIsoSurfaceOutOfCore);

    // create before the instance of the Serialization manager, which will register signals.
    m_SerializationHandle = mafInitializeModule(SERIALIZATION_LIBRARY_NAME);
    QVERIFY(m_SerializationHandle != NULL);

    mafRegisterObject(mafPluginOutOfCore::mafExternalDataCodecVolume);

    // register external codec
    QString encodeType = "VOLUME_LOD";
    QString codec = "mafPluginOutOfCore::mafExternalDataCodecVolume";
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, encodeType));
    argList.append(mafEventArgument(QString, codec));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.plugCodec", mafEventTypeLocal, &argList);

    // initialize graphic resources
    initializeGraphicResources();

    // initialize the volumes
    encodeVolume();
    decodeVolume();

    //Insert data into VME
    m_VME = mafNEW(mafResources::mafVME);
    mafResources::mafDataSet *dataSet = mafNEW(mafResources::mafDataSet);
    dataSet->setDataValue(&m_Volume);
    m_VME->dataSetCollection()->insertItem(dataSet, 0);
    mafDEL(dataSet);
}

void mafPipeVisualVTKIsoSurfaceOutOfCoreTest::cleanupTestCase() {
    //here delete vtk stuff
    QFile::remove(m_VolumeFileName);
    QFile::remove(m_Volume->fileName());
    mafDEL(m_Volume);
    mafDEL(m_VME);
    mafMessageHandler::instance()->shutdown();
    shutdownGraphicResources();
    mafShutdownModule(m_SerializationHandle);
    mafEventBusManager::instance()->shutdown();
}

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

void mafPipeVisualVTKIsoSurfaceOutOfCoreTest::encodeVolume() {
    // create the volume (unsigned short)
    m_VolumeFileName  = QString(MAF_DATA_DIR) + "/VTK/graySphere.lod";
    mafVolume *volume = mafNEW(mafPluginOutOfCore::mafVolume);
    
    int dimensions[3] = { 512, 512, 512 };
    float spacing[3]  = { 0.1f, 0.1f, 0.1f };
    volume->setDataType(mafPluginOutOfCore::mafVolUnsignedShort);
    volume->setBitsStored(12);
    volume->setBigEndian(false);
    volume->setComponentNum(1);
    volume->setOriginalDimensions(dimensions);
    volume->setSpacing(spacing);
    volume->setDimensions(dimensions);
    volume->setLevelNum(2);                   // level number for 4M memory limit
    volume->setMemoryLimit(4 * 1024);         // 4M memory limit
    volume->setFileName("graySphere.raw");    // External data file name
    // dataValue
    ushort *data = new ushort[dimensions[2] * dimensions[1] * dimensions[0]];
    for (int z = 0; z < dimensions[2]; ++z) {
        double dz = z - dimensions[2] * 0.5;
        for (int y = 0; y < dimensions[1]; ++y) {
            double dy = y - dimensions[1] * 0.5;
            for (int x = 0; x < dimensions[0]; ++x) {
                double dx = x - dimensions[0] * 0.5;
                int radius = (int)sqrt(dx * dx + dy * dy + dz * dz);
                int index  = (z * dimensions[1] + y ) * dimensions[0] + x;
                int value  = 512 - radius * 2;
                data[index]= (value > 0 ? value : 0);
            }
        }
    }
    volume->setDataValue(data);

    // encode the volume
    mafMementoVolume *memento = (mafMementoVolume*)volume->createMemento();
    QVERIFY(memento);

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafMemento *, memento));
    argList.append(mafEventArgument(QString, m_VolumeFileName));
    argList.append(mafEventArgument(QString, memento->encodeType()));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.save", mafEventTypeLocal, &argList);

    // meta data file
    QVERIFY(QFile::exists(m_VolumeFileName));
    QFileInfo metaFileInfo(m_VolumeFileName);
    QVERIFY(metaFileInfo.size() > 0);

    // volume data file
    QVERIFY(QFile::exists(QString(MAF_DATA_DIR) + "/VTK/" + volume->fileName()));
    QFileInfo dataFileInfo(QString(MAF_DATA_DIR) + "/VTK/" + volume->fileName());
    QVERIFY(dataFileInfo.size() > 0);

    mafDEL(memento);
    mafDEL(volume);
}

void mafPipeVisualVTKIsoSurfaceOutOfCoreTest::decodeVolume() {
    QString encodeType = "RAW";
    mafCore::mafMemento *memento = 0;

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, m_VolumeFileName));
    argList.append(mafEventArgument(QString, encodeType));
    QGenericReturnArgument retVal = mafEventReturnArgument(mafCore::mafMemento*, memento);
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.load", mafEventTypeLocal, &argList, &retVal);

    // verify memento
    QVERIFY(memento);
    m_Volume = mafNEW(mafPluginOutOfCore::mafVolume);
    m_Volume->setMemento(memento);
    mafDEL(memento);
}

void mafPipeVisualVTKIsoSurfaceOutOfCoreTest::updatePipeTest() {
    // load the initial volume data using the level = 3
    m_Volume->setMemoryLimit(16 * 1024);
    m_Volume->updateDataValue();

    mafPipeVisualVTKIsoSurfaceOutOfCore *pipe;
    pipe = mafNEW(mafPluginVTK::mafPipeVisualVTKIsoSurfaceOutOfCore);
    pipe->setInput(m_VME);
    pipe->setGraphicObject(m_RenderWidget);
    pipe->setContourValue(QString::number(20));
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
    
    // change the contour value from 20 to 180
    for (int i = 20; i < 200; i += 20) {
        pipe->setContourValue(QString::number(i));
        pipe->updatePipe();
        QTest::qSleep(500);
    }

    // enlarge the memory limit using the level = 2
    m_Volume->setMemoryLimit(64 * 1024);
    m_Volume->updateDataValue();

    pipe->setContourValue(QString::number(20));
    pipe->updatePipe();
    m_Renderer->ResetCamera();
    for (int i = 20; i < 200; i += 20) {
        pipe->setContourValue(QString::number(i));
        pipe->updatePipe();
        QTest::qSleep(2500);
    }

    // set the size of ROI, using the level = 1
    int startPos[3] = { 100, 100, 100 };
    int dimensions[3] = { 256, 232, 280 };
    m_Volume->setMemoryLimit(128 * 1024);
    m_Volume->setUpdatedStartPos(startPos);
    m_Volume->setUpdatedDimensions(dimensions);
    m_Volume->updateDataValue();

    for (int i = 200; i < 400; i += 20) {
        pipe->setContourValue(QString::number(i));
        pipe->updatePipe();
        QTest::qSleep(500);
    }

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

    // update the mafVolume data
    m_Volume->setMemoryLimit(64 * 1024);
    m_Volume->updateDataValue();
    int defaultPos[3] = { 0, 0, 0 };
    m_Volume->setUpdatedStartPos(defaultPos);
    m_Volume->setUpdatedDimensions(m_Volume->originalDimensions());
    m_Volume->updateDataValue();


    //! <snippet>
    mafPipeVisual *visualPipe = (mafPipeVisual *)mafNEWFromString("mafPluginVTK::mafPipeVisualVTKIsoSurfaceOutOfCore");
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
    QTest::qSleep(22000);

    mafDEL(visualPipe);
    pluginManager->shutdown();
}

MAF_REGISTER_TEST(mafPipeVisualVTKIsoSurfaceOutOfCoreTest);
#include "mafPipeVisualVTKIsoSurfaceOutOfCoreTest.moc"
