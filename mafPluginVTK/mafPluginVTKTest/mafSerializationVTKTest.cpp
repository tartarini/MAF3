/*
 *  mafSerializationVTKTest.cpp
 *  mafPluginVTKTest
 *
 *  Created by Roberto Mucci on 26/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafContainer.h>
#include <mafMementoVME.h>
#include <mafExternalDataCodecVTK.h>
#include <mafEventBusManager.h>

#include <vtkDataSet.h>
#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkAlgorithmOutput.h>

#ifdef WIN32
    #define SERIALIZATION_LIBRARY_NAME "mafSerialization_d.dll"
#else
    #ifdef __APPLE__
        #define SERIALIZATION_LIBRARY_NAME "libmafSerialization_debug.dylib"
    #else
        #define SERIALIZATION_LIBRARY_NAME "libmafSerialization_debug.so"
    #endif
#endif


using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;
using namespace mafEventBus;

/**
Class name: testCustomManager
This class defines the custom manager class used to test the mafSerialization manager interface API.
*/
class testCustomManager : public mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor
    testCustomManager(const mafString code_location = "");

public slots:
    /// observer needed to receive the 'extDataLoaded' signal
    void createdExtData(mafCore::mafContainerInterface *data);
};

testCustomManager::testCustomManager(QString code_location) : mafObjectBase(code_location) {
    mafRegisterLocalCallback("maf.local.serialization.extDataImported", this , "createdExtData(mafCore::mafContainerInterface *)");
}

void testCustomManager::createdExtData(mafCore::mafContainerInterface *data) {
    mafMsgDebug("%s", mafTr("External data loaded!!").toAscii().data());
    QVERIFY(data != NULL);


    mafContainer<vtkAlgorithmOutput> *dataSet = mafContainerPointerTypeCast(vtkAlgorithmOutput, data);
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
    mafDEL(data);
}


/**
 Class name: mafSerializationVTKTest
 This class implements the test suite to test serialization mechanism with VTK lib.
 */

class mafSerializationVTKTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        // Create before the instance of the Serialization manager, which will register signals.
        bool res(false);
        res = mafInitializeModule(SERIALIZATION_LIBRARY_NAME);
        QVERIFY(res);

        mafEventBusManager::instance();
        m_CustomManager = mafNEW(testCustomManager);

        //Create two codec
        m_Codec = mafNEW(mafPluginVTK::mafExternalDataCodecVTK);

        // Create a polydata.
        m_DataSource = vtkCubeSource::New();
        m_DataSource->SetXLength(5);
        m_DataSource->SetYLength(3);
        m_DataSource->SetZLength(8);
        m_DataSourceContainer = m_DataSource->GetOutputPort(0);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_CustomManager);
        mafDEL(m_Codec);
        m_DataSource->Delete();
        mafEventBusManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafSerializationVTKTest allocation test case.
    void mafSerializationVTKAllocationTest();

    /// mafSerializationVTKTest econde test case.
    void mafSerializationVTKSaveTest();

private:
    mafExternalDataCodecVTK *m_Codec; ///< Test var.
    mafDataStream m_OutputStream; ///< Test var.
    testCustomManager *m_CustomManager; ///< Manager test var

    vtkCubeSource *m_DataSource; ///< Source data for the test suite.
    mafContainer<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
};

void mafSerializationVTKTest::mafSerializationVTKAllocationTest() {
    QVERIFY(m_Codec != NULL);
}

void mafSerializationVTKTest::mafSerializationVTKSaveTest() {
    // Create the temporary file into the temp directory of the current user.
    mafString test_file;
    test_file = QDir::tempPath();
    test_file.append("/maf3Logs");
    QDir log_dir(test_file);
    if(!log_dir.exists()) {
        log_dir.mkpath(test_file);
    }
    test_file.append("/testVTKFile.vtk");
    mafFile::remove(test_file);

    //mafId plug_codec_id = mafIdProvider::instance()->idValue("maf.local.serialization.plugCodec");
    mafString plug_codec_id = "maf.local.serialization.plugCodec";
    mafString obj_type("vtkAlgorithmOutput");
    mafString vtk = "VTK";
    mafString codecVTK = "mafPluginVTK::mafExternalDataCodecVTK";

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafString, obj_type));
    argList.append(mafEventArgument(mafString, vtk));
    argList.append(mafEventArgument(mafString, codecVTK));
    mafEventBusManager::instance()->notifyEvent(plug_codec_id, mafEventTypeLocal, &argList);

    mafString encodeType = "VTK";
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafContainerInterface *, &m_DataSourceContainer));
    argList.append(mafEventArgument(mafString, test_file));
    argList.append(mafEventArgument(mafString, encodeType));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.export", mafEventTypeLocal, &argList);

    QVERIFY(mafFile::exists(test_file));
    QFileInfo fInfo3(test_file);
    QVERIFY(fInfo3.size() > 0);

    encodeType = "VTK";
    argList.clear();
    argList.append(mafEventArgument(mafString, test_file));
    argList.append(mafEventArgument(mafString, encodeType));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.import", mafEventTypeLocal, &argList);

    mafFile::remove(test_file);
}

MAF_REGISTER_TEST(mafSerializationVTKTest);
#include "mafSerializationVTKTest.moc"
