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
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkFloatArray.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>

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
    mafRegisterLocalCallback("EXTDATA_IMPORTED", this , "createdExtData(mafCore::mafContainerInterface *)");
}

void testCustomManager::createdExtData(mafCore::mafContainerInterface *data) {
    mafMsgDebug("%s", mafTr("External data loaded!!").toAscii().data());
    QVERIFY(data != NULL);

    mafContainer<vtkPolyData> *dataSet = mafContainerPointerTypeCast(vtkPolyData, data);

    double boundsIn[6] = {0,6,0,6,0,5};
    double boundsOut[6];
    (*dataSet)->GetBounds(boundsOut);
    QCOMPARE(boundsIn[0], boundsOut[0]);
    QCOMPARE(boundsIn[1], boundsOut[1]);
    QCOMPARE(boundsIn[2], boundsOut[2]);
    QCOMPARE(boundsIn[3], boundsOut[3]);
    QCOMPARE(boundsIn[4], boundsOut[4]);
    QCOMPARE(boundsIn[5], boundsOut[5]);


    int i;
    double x[8][3]={{0,0,0}, {5,0,0}, {5,5,0}, {0,5,0},
                        {0,0,5}, {5,0,1}, {6,5,2}, {0,6,5}};
    vtkPoints *points =(*dataSet)->GetPoints();
    for (i=0; i<8; i++) {
        double p[3];
        points->GetPoint(i, p);
        QVERIFY(p[0] == x[i][0] && p[1] == x[i][1] && p[2] == x[i][2]);
    }

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
        // Create before the instance of the Serialization manager, which will register signals.
        bool res(false);
        res = mafInitializeModule(SERIALIZATION_LIBRARY_NAME);
        QVERIFY(res);

        mafEventBusManager::instance();
        m_CustomManager = mafNEW(testCustomManager);

        //Create two codec
        m_Codec = mafNEW(mafPluginVTK::mafExternalDataCodecVTK);

        // Create a polydata.
        vtkPolyData *surface = vtkPolyData::New();
        vtkPoints *points = vtkPoints::New();
        vtkCellArray *polys = vtkCellArray::New();
        vtkFloatArray *scalars = vtkFloatArray::New();

        int i;
        static float x[8][3]={{0,0,0}, {5,0,0}, {5,5,0}, {0,5,0},
                        {0,0,5}, {5,0,1}, {6,5,2}, {0,6,5}};
        static vtkIdType pts[6][4]={{0,1,2,3}, {4,5,6,7}, {0,1,5,4},
                        {1,2,6,5}, {2,3,7,6}, {3,0,4,7}};
        for (i=0; i<8; i++) points->InsertPoint(i,x[i]);
        for (i=0; i<6; i++) polys->InsertNextCell(4,pts[i]);
        for (i=0; i<8; i++) scalars->InsertTuple1(i,i);

        // We now assign the pieces to the vtkPolyData.
        surface->SetPoints(points);
        surface->SetPolys(polys);
        surface->GetPointData()->SetScalars(scalars);
        points->Delete();
        polys->Delete();
        scalars->Delete();

        m_Container.setDestructionFunction(&vtkPolyData::Delete);
        m_Container.setExternalCodecType("mafPluginVTK::mafExternalDataCodecVTK");
        m_Container.setExternalDataType("vtkPolydata");
        m_Container = surface;
        mafDataSet *dataSet = mafNEW(mafResources::mafDataSet);
        dataSet->setDataValue(&m_Container);
        mafDEL(dataSet);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_CustomManager);
        mafDEL(m_Codec);
        mafCoreSingletons::mafSingletonsShutdown();
        mafEventBusManager::instance()->shutdown();
    }

    /// mafSerializationVTKTest allocation test case.
    void mafSerializationVTKAllocationTest();

    /// mafSerializationVTKTest econde test case.
    void mafSerializationVTKSaveTest();

private:
    mafExternalDataCodecVTK *m_Codec; ///< Test var.
    mafContainer<vtkPolyData> m_Container; ///< Test var.
    mafDataStream m_OutputStream; ///< Test var.
    testCustomManager *m_CustomManager; ///< Manager test var
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

    //mafId plug_codec_id = mafIdProvider::instance()->idValue("PLUG_CODEC");
    mafString plug_codec_id = "PLUG_CODEC";
    mafString obj_type("vtkPolyData");
    mafString vtk = "VTK";
    mafString codecVTK = "mafPluginVTK::mafExternalDataCodecVTK";

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafString, obj_type));
    argList.append(mafEventArgument(mafString, vtk));
    argList.append(mafEventArgument(mafString, codecVTK));
    mafEventBusManager::instance()->notifyEvent(plug_codec_id, mafEventTypeLocal, &argList);

    mafString encodeType = "VTK";
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafContainerInterface *, &m_Container));
    argList.append(mafEventArgument(mafString, test_file));
    argList.append(mafEventArgument(mafString, encodeType));
    mafEventBusManager::instance()->notifyEvent("EXPORT", mafEventTypeLocal, &argList);

    QVERIFY(mafFile::exists(test_file));
    QFileInfo fInfo3(test_file);
    QVERIFY(fInfo3.size() > 0);


    encodeType = "VTK";
    argList.clear();
    argList.append(mafEventArgument(mafString, test_file));
    argList.append(mafEventArgument(mafString, encodeType));
    mafEventBusManager::instance()->notifyEvent("IMPORT", mafEventTypeLocal, &argList);

    mafFile::remove(test_file);
}

MAF_REGISTER_TEST(mafSerializationVTKTest);
#include "mafSerializationVTKTest.moc"
