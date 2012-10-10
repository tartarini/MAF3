/*
 *  mafSerializationXMLExtDataTest.cpp
 *  mafPluginVTKTest
 *
 *  Created by Roberto Mucci on 26/03/10.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafCoreSingletons.h>
#include <mafTestSuite.h>
#include <mafProxy.h>
#include <mafMementoVME.h>
#include <mafExternalDataCodecVTK.h>
#include <mafEventBusManager.h>
#include <mafVMEManager.h>
#include <mafDataBoundaryAlgorithmVTK.h>
#include <mafVME.h>
#include <mafDataSet.h>


#include <vtkDataSet.h>
#include <vtkPolyData.h>
#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkAlgorithmOutput.h>

#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

#ifdef WIN32
#define SERIALIZATION_LIBRARY_NAME "mafSerialization.dll"
#else
#ifdef __APPLE__
#define SERIALIZATION_LIBRARY_NAME "mafSerialization.dylib"
#else
#define SERIALIZATION_LIBRARY_NAME "mafSerialization.so"
#endif
#endif


using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;
using namespace mafEventBus;


/**
 Class name: mafSerializationXMLExtDataTest
 This class implements the test suite to test serialization mechanism with VTK lib.
 */

class mafSerializationXMLExtDataTest : public QObject {
    Q_OBJECT
    
    private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        
        // Create before the instance of the Serialization manager, which will register signals.
        m_SerializationHandle = mafInitializeModule(SERIALIZATION_LIBRARY_NAME);
        QVERIFY(m_SerializationHandle != NULL);
        
        mafEventBusManager::instance();
        m_VMEManager = mafVMEManager::instance();
        
        //Get hierarchy
        mafCore::mafHierarchyPointer hierarchy;
        QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafHierarchyPointer, hierarchy);
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.request", mafEventTypeLocal, NULL, &ret_val);
        
        //Select root
        mafObject *root;
        ret_val = mafEventReturnArgument(mafCore::mafObject *, root);
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.root", mafEventTypeLocal, NULL, &ret_val);
        
        //Create two codec
        m_Codec = mafNEW(mafPluginVTK::mafExternalDataCodecVTK);
        
        // Create a polydata.
        m_DataSource = vtkCubeSource::New();
        m_DataSource->SetXLength(5);
        m_DataSource->SetYLength(3);
        m_DataSource->SetZLength(8);
        
        m_DataSourceContainer.setExternalCodecType("VTK");
        m_DataSourceContainer.setClassTypeNameFunction(vtkClassTypeNameExtract);
        m_DataSourceContainer = m_DataSource->GetOutputPort(0);
        
        //Insert data into VME
        m_Vme = mafNEW(mafResources::mafVME);
        m_DataSetCube = mafNEW(mafResources::mafDataSet);
        mafDataBoundaryAlgorithmVTK *boundaryAlgorithm;
        boundaryAlgorithm = mafNEW(mafDataBoundaryAlgorithmVTK);
        m_DataSetCube->setBoundaryAlgorithm(boundaryAlgorithm);
        m_DataSetCube->setDataValue(&m_DataSourceContainer);
        
        // Second test VME
        m_DataSourceMoved = vtkCubeSource::New();
        m_DataSourceMoved->SetXLength(5);
        m_DataSourceMoved->SetYLength(3);
        m_DataSourceMoved->SetZLength(8);
        
        vtkTransform *t = vtkTransform::New();
        t->RotateZ(50);
        t->RotateX(50);
        t->Update();
        
        m_PDataFilter = vtkTransformPolyDataFilter::New();
        m_PDataFilter->SetTransform(t);
        m_PDataFilter->SetInputConnection(m_DataSourceMoved->GetOutputPort(0));
        m_PDataFilter->Update();
        t->Delete();
        
        m_DataSourceContainerMoved.setExternalCodecType("VTK");
        m_DataSourceContainerMoved.setClassTypeNameFunction(vtkClassTypeNameExtract);
        m_DataSourceContainerMoved = m_PDataFilter->GetOutputPort(0);
        m_DataSetCubeMoved = mafNEW(mafResources::mafDataSet);
        
        mafDataBoundaryAlgorithmVTK *boundaryAlgorithm1;
        boundaryAlgorithm1 = mafNEW(mafDataBoundaryAlgorithmVTK);
        m_DataSetCubeMoved->setBoundaryAlgorithm(boundaryAlgorithm1);
        m_DataSetCubeMoved->setDataValue(&m_DataSourceContainerMoved);
        
        m_Vme->dataSetCollection()->insertItem(m_DataSetCube, 0);
        m_Vme->dataSetCollection()->insertItem(m_DataSetCubeMoved, 1);
    }
    
    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Codec);
        mafDEL(m_DataSetCube);
        mafDEL(m_DataSetCubeMoved);
        mafDEL(m_Vme);
        m_PDataFilter->Delete();
        m_DataSource->Delete();
        m_DataSourceMoved->Delete();
        m_VMEManager->shutdown();
        mafShutdownModule(m_SerializationHandle);
        mafEventBusManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }
    
    /// mafSerializationXMLExtDataTest allocation test case.
    void mafSerializationVTKAllocationTest();
    
    /// mafSerializationXMLExtDataTest econde test case.
    void mafSerializationVTKSaveTest();
    
private:
    mafExternalDataCodecVTK *m_Codec; ///< Test var.
    vtkCubeSource *m_DataSource;
    vtkCubeSource *m_DataSourceMoved;
    QDataStream m_OutputStream; ///< Test var.
    mafVME *m_Vme; ///< Test var.
    mafProxy<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
    mafProxy<vtkAlgorithmOutput> m_DataSourceContainerMoved; ///< Container of the Data Source
    vtkTransformPolyDataFilter *m_PDataFilter; ///< Filter used to transform the bounding box.
    mafResources::mafDataSet *m_DataSetCube;
    mafResources::mafDataSet *m_DataSetCubeMoved;
    mafVMEManager *m_VMEManager;
    QLibrary *m_SerializationHandle;
    
};

void mafSerializationXMLExtDataTest::mafSerializationVTKAllocationTest() {
    QVERIFY(m_Codec != NULL);
}


void mafSerializationXMLExtDataTest::mafSerializationVTKSaveTest() {
    QString test_dir;
    test_dir = QDir::tempPath();
    test_dir.append("/maf3Logs");
    
    QDir log_dir(test_dir);
    log_dir.setFilter(QDir::Files);
    QStringList list = log_dir.entryList();
    int i = 0;
    
    //remove files crested by test
    for (; i < list.size(); ++i) {
        QString fileName = test_dir;
        fileName.append("/");
        fileName.append(list.at(i));
        QFile::remove(fileName);
    }
    
    // Create the temporary file into the temp directory of the current user.
    QString test_file = test_dir;
    test_file.append("/testExtFile.xml");
    qDebug() << test_file;
    
    QString plug_codec_id = "maf.local.serialization.plugCodec";
    QString encodeType = "XML";
    QString codec = "mafSerialization::mafCodecXML";
    
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, encodeType));
    argList.append(mafEventArgument(QString, codec));
    mafEventBusManager::instance()->notifyEvent(plug_codec_id, mafEventTypeLocal, &argList);
    
    encodeType = "VTK";
    codec = "mafPluginVTK::mafExternalDataCodecVTK";
    
    argList.clear();
    argList.append(mafEventArgument(QString, encodeType));
    argList.append(mafEventArgument(QString, codec));
    mafEventBusManager::instance()->notifyEvent(plug_codec_id, mafEventTypeLocal, &argList);
    
    //Save VME with ASCII dataSet
    mafMementoVME *mementoVME = (mafMementoVME *)m_Vme->createMemento();
    QVERIFY(mementoVME != NULL);    
    
    argList.clear();
    encodeType = "XML";
    argList.clear();
    mafCore::mafMemento *mem = mementoVME;
    argList.append(mafEventArgument(mafCore::mafMemento *, mem));
    argList.append(mafEventArgument(QString, test_file));
    argList.append(mafEventArgument(QString, encodeType));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.save", mafEventTypeLocal, &argList);
    
    QVERIFY(QFile::exists(test_file));
    QFileInfo fInfo3(test_file);
    QVERIFY(fInfo3.size() > 0);
    
    mafCore::mafMemento *memento;
    encodeType = "XML";
    argList.clear();
    argList.append(mafEventArgument(QString, test_file));
    argList.append(mafEventArgument(QString, encodeType));
    QGenericReturnArgument retVal = mafEventReturnArgument(mafCore::mafMemento*, memento);
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.load", mafEventTypeLocal, &argList, &retVal);
    
    QVERIFY(memento != NULL);
    
    mafVME *returnVME = mafNEW(mafResources::mafVME);
    returnVME->setMemento(memento, false);
    
    //Now load dataValue
    mafDataSet *data = returnVME->dataSetCollection()->itemAt(0);
    
    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, data->dataValue());
    vtkPolyDataMapper *sphereMapper = vtkPolyDataMapper::New();
    sphereMapper->SetInputConnection(*dataSet);
    
    double boundsIn[6] = {-2.5,2.5,-1.5,1.5,-4,4};
    double boundsOut[6];
    sphereMapper->GetBounds(boundsOut);
    QCOMPARE(boundsIn[0], boundsOut[0]);
    QCOMPARE(boundsIn[1], boundsOut[1]);
    QCOMPARE(boundsIn[2], boundsOut[2]);
    QCOMPARE(boundsIn[3], boundsOut[3]);
    QCOMPARE(boundsIn[4], boundsOut[4]);
    QCOMPARE(boundsIn[5], boundsOut[5]);
    
    sphereMapper->Delete();
    mafDEL(returnVME);
    mafDEL(mementoVME);
    mafDEL(memento);
    i = 0;
    //remove files crested by test
    for (; i < list.size(); ++i) {
        QString fileName = test_dir;
        fileName.append("/");
        fileName.append(list.at(i));
        QFile::remove(fileName);
    }
    
}

MAF_REGISTER_TEST(mafSerializationXMLExtDataTest);
#include "mafSerializationExtDataTest.moc"
