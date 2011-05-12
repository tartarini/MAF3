/*
 *  mafSerializationRawASCIIExtDataTest.cpp
 *  mafPluginVTKTest
 *
 *  Created by Roberto Mucci on 26/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafCoreSingletons.h>
#include <mafTestSuite.h>
#include <mafProxy.h>
#include <mafMementoVME.h>
#include <mafExternalDataCodecVTK.h>
#include <mafEventBusManager.h>
#include <mafDataBoundaryAlgorithmVTK.h>


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
Class name: testExtRawASCIICustomManager
This class defines the custom manager class used to test the mafSerialization manager interface API.
*/
class testExtRawASCIICustomManager : public mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor
    testExtRawASCIICustomManager(const QString code_location = "");

public slots:
    /// observer needed to receive the 'extDataLoaded' signal
    void createdMemento(mafCore::mafMemento *memento);
};

testExtRawASCIICustomManager::testExtRawASCIICustomManager(QString code_location) : mafObjectBase(code_location) {
    mafRegisterLocalCallback("maf.local.serialization.mementoLoaded", this, "createdMemento(mafCore::mafMemento *)");
}

void testExtRawASCIICustomManager::createdMemento(mafCore::mafMemento *memento) {
    qDebug("%s", mafTr("memento loaded!!").toAscii().data());
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
}

/**
 Class name: mafSerializationRawASCIIExtDataTest
 This class implements the test suite to test serialization mechanism with VTK lib.
 */


class mafSerializationRawASCIIExtDataTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        // Create before the instance of the Serialization manager, which will register signals.
        bool res(false);
        res = mafInitializeModule(SERIALIZATION_LIBRARY_NAME);
        QVERIFY(res);

        mafEventBusManager::instance();
        m_CustomManager = mafNEW(testExtRawASCIICustomManager);

        //Create two codec
        m_Codec = mafNEW(mafPluginVTK::mafExternalDataCodecVTK);

        // Create a polydata.
        m_DataSource = vtkCubeSource::New();
        m_DataSource->SetXLength(5);
        m_DataSource->SetYLength(3);
        m_DataSource->SetZLength(8);

        m_DataSourceContainer.setExternalCodecType("VTK");
        m_DataSourceContainer.setExternalDataType("vtkAlgorithmOutput");
        m_DataSourceContainer = m_DataSource->GetOutputPort(0);

        //Insert data into VME
        m_Vme = mafNEW(mafResources::mafVME);
        m_DataSetCube = mafNEW(mafResources::mafDataSet);
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
        m_DataSourceContainerMoved.setExternalDataType("vtkAlgorithmOutput");
        m_DataSourceContainerMoved = m_PDataFilter->GetOutputPort(0);
        m_DataSetCubeMoved = mafNEW(mafResources::mafDataSet);
        m_DataSetCubeMoved->setDataValue(&m_DataSourceContainerMoved);

        m_Vme->dataSetCollection()->insertItem(m_DataSetCube, 0);
        mafDataBoundaryAlgorithmVTK *boundaryAlgorithm;
        boundaryAlgorithm = mafNEW(mafDataBoundaryAlgorithmVTK);
        m_DataSetCube->setBoundaryAlgorithm(boundaryAlgorithm);
//        mafProxyInterface *boundary0 = m_Vme->dataSetCollection()->itemAt(0)->dataBoundary();

        m_Vme->dataSetCollection()->insertItem(m_DataSetCubeMoved, 1);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_CustomManager);
        mafDEL(m_Codec);
        mafDEL(m_DataSetCube);
        mafDEL(m_DataSetCubeMoved);
        mafDEL(m_Vme);
        m_PDataFilter->Delete();
        m_DataSource->Delete();
        m_DataSourceMoved->Delete();
        mafEventBusManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafSerializationRawASCIIExtDataTest allocation test case.
    void mafSerializationVTKAllocationTest();

    /// mafSerializationRawASCIIExtDataTest econde test case.
    void mafSerializationVTKSaveTest();

private:
    mafExternalDataCodecVTK *m_Codec; ///< Test var.
    vtkCubeSource *m_DataSource;
    vtkCubeSource *m_DataSourceMoved;
    QDataStream m_OutputStream; ///< Test var.
    testExtRawASCIICustomManager *m_CustomManager; ///< Manager test var
    mafVME *m_Vme; ///< Test var.
    mafProxy<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
    mafProxy<vtkAlgorithmOutput> m_DataSourceContainerMoved; ///< Container of the Data Source
    vtkTransformPolyDataFilter *m_PDataFilter; ///< Filter used to transform the bounding box.
    mafResources::mafDataSet *m_DataSetCube;
    mafResources::mafDataSet *m_DataSetCubeMoved;
};

void mafSerializationRawASCIIExtDataTest::mafSerializationVTKAllocationTest() {
    QVERIFY(m_Codec != NULL);
}


void mafSerializationRawASCIIExtDataTest::mafSerializationVTKSaveTest() {
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
    test_file.append("/testExtFile.raw");
    qDebug() << test_file;

    QString plug_codec_id = "maf.local.serialization.plugCodec";
    QString obj_type("mafResources::mafVME");
    QString encodeType = "RAW_ASCII";
    QString codec = "mafSerialization::mafCodecRawASCII";

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, obj_type));
    argList.append(mafEventArgument(QString, encodeType));
    argList.append(mafEventArgument(QString, codec));
    mafEventBusManager::instance()->notifyEvent(plug_codec_id, mafEventTypeLocal, &argList);

    
    obj_type = "vtkAlgorithmOutput";
    encodeType = "VTK";
    codec = "mafPluginVTK::mafExternalDataCodecVTK";

    argList.clear();
    argList.append(mafEventArgument(QString, obj_type));
    argList.append(mafEventArgument(QString, encodeType));
    argList.append(mafEventArgument(QString, codec));
    mafEventBusManager::instance()->notifyEvent(plug_codec_id, mafEventTypeLocal, &argList);

    //Save VME with ASCII dataSet
    mafMementoVME *mementoVME = (mafMementoVME *)m_Vme->createMemento();
    QVERIFY(mementoVME != NULL);

    argList.clear();
    encodeType = "RAW_ASCII";
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafMemento *, mementoVME));
    argList.append(mafEventArgument(QString, test_file));
    argList.append(mafEventArgument(QString, encodeType));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.save", mafEventTypeLocal, &argList);

    QVERIFY(QFile::exists(test_file));
    QFileInfo fInfo3(test_file);
    QVERIFY(fInfo3.size() > 0);


    encodeType = "RAW_ASCII";
    argList.clear();
    argList.append(mafEventArgument(QString, test_file));
    argList.append(mafEventArgument(QString, encodeType));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.load", mafEventTypeLocal, &argList);

    mafDEL(mementoVME);

    i = 0;
    //remove files crested by test
    for (; i < list.size(); ++i) {
        QString fileName = test_dir;
        fileName.append("/");
        fileName.append(list.at(i));
        QFile::remove(fileName);
    }

}

MAF_REGISTER_TEST(mafSerializationRawASCIIExtDataTest);
#include "mafSerializationRawASCIIExtDataTest.moc"
