/*
 *  mafSerializationExtDataTest.cpp
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

#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

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
Class name: testExtCustomManager
This class defines the custom manager class used to test the mafSerialization manager interface API.
*/
class testExtCustomManager : public mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor
    testExtCustomManager(const mafString code_location = "");

public slots:
    /// observer needed to receive the 'extDataLoaded' signal
    void createdMemento(mafCore::mafMemento *memento);
};

testExtCustomManager::testExtCustomManager(QString code_location) : mafObjectBase(code_location) {
    //mafCore::mafId id = mafIdProvider::instance()->idValue("maf.local.serialization.mementoLoaded");
    mafRegisterLocalCallback("maf.local.serialization.mementoLoaded", this, "createdMemento(mafCore::mafMemento *)");

}

void testExtCustomManager::createdMemento(mafCore::mafMemento *memento) {
    mafMsgDebug("%s", mafTr("memento loaded!!").toAscii().data());
    QVERIFY(memento != NULL);

    mafVME *returnVME = mafNEW(mafResources::mafVME);
    returnVME->setMemento(memento);

    //Now load dataValue
    returnVME->updateData();


    mafDataSet *data = returnVME->dataSetCollection()->itemAt(1);

    mafContainer<vtkPolyData> *dataSet = mafContainerPointerTypeCast(vtkPolyData, data->dataValue());

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
    mafDEL(memento);
    mafDEL(returnVME);
}

/**
 Class name: mafSerializationExtDataTest
 This class implements the test suite to test serialization mechanism with VTK lib.
 */


class mafSerializationExtDataTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafResourcesRegistration::registerResourcesObjects();
        // Create before the instance of the Serialization manager, which will register signals.
        bool res(false);
        res = mafInitializeModule(SERIALIZATION_LIBRARY_NAME);
        QVERIFY(res);

        mafEventBusManager::instance();
        m_CustomManager = mafNEW(testExtCustomManager);

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
        //Insert data into VME
        m_Vme = mafNEW(mafResources::mafVME);
        mafDataSet *dataSetCube = mafNEW(mafResources::mafDataSet);
        dataSetCube->setDataValue(&m_Container);

        // Second test VME
        vtkPolyData *surfaceMoved = vtkPolyData::New();
        surfaceMoved->DeepCopy(surface);
        surfaceMoved->Update();

        vtkTransform *t = vtkTransform::New();
        t->RotateZ(50);
        t->RotateX(50);
        t->Update();

        vtkTransformPolyDataFilter *ptf = vtkTransformPolyDataFilter::New();
        ptf->SetTransform(t);
        ptf->SetInput(surfaceMoved);
        ptf->Update();
        t->Delete();

        surfaceMoved->DeepCopy(ptf->GetOutput());
        surfaceMoved->Update();
        ptf->Delete();

        m_ContainerMoved.setDestructionFunction(&vtkPolyData::Delete);
        m_ContainerMoved.setExternalCodecType("mafPluginVTK::mafExternalDataCodecVTK");
        m_ContainerMoved.setExternalDataType("vtkPolydata");
        m_ContainerMoved = surfaceMoved;
        mafDataSet *dataSetMoved = mafNEW(mafResources::mafDataSet);
        dataSetMoved->setDataValue(&m_ContainerMoved);

        m_Vme->dataSetCollection()->insertItem(dataSetCube, 1);
        m_Vme->dataSetCollection()->insertItem(dataSetMoved, 2);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_CustomManager);
        mafDEL(m_Codec);
        mafDEL(m_Vme);
        mafCoreSingletons::mafSingletonsShutdown();
        mafEventBusManager::instance()->shutdown();
    }

    /// mafSerializationExtDataTest allocation test case.
    void mafSerializationVTKAllocationTest();

    /// mafSerializationExtDataTest econde test case.
    void mafSerializationVTKSaveTest();

private:
    mafExternalDataCodecVTK *m_Codec; ///< Test var.
    mafContainer<vtkPolyData> m_Container; ///< Test var.
    mafContainer<vtkPolyData> m_ContainerMoved;///< Test var.
    mafDataStream m_OutputStream; ///< Test var.
    testExtCustomManager *m_CustomManager; ///< Manager test var
    mafVME *m_Vme; ///< Test var.
};

void mafSerializationExtDataTest::mafSerializationVTKAllocationTest() {
    QVERIFY(m_Codec != NULL);
}


void mafSerializationExtDataTest::mafSerializationVTKSaveTest() {
    // Create the temporary file into the temp directory of the current user.
    mafString test_dir;
    test_dir = QDir::tempPath();
    test_dir.append("/maf3Logs");
    mafString test_file = test_dir;
    test_file.append("/testExtFile.xml");

    //mafId plug_codec_id = mafIdProvider::instance()->idValue("PLUG_CODEC");
    mafString plug_codec_id = "PLUG_CODEC";
    mafString obj_type("mafResources::mafVME");
    mafString vtk = "XML";
    mafString codecVTK = "mafSerialization::mafCodecXML";

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafString, obj_type));
    argList.append(mafEventArgument(mafString, vtk));
    argList.append(mafEventArgument(mafString, codecVTK));
    mafEventBusManager::instance()->notifyEvent(plug_codec_id, mafEventTypeLocal, &argList);

    mafMemento *m = m_Vme->mafResource::createMemento();
    mafMementoVME *mementoVME = new mafMementoVME(m_Vme, mafCodeLocation);
    QVERIFY(mementoVME != NULL);
    m->setParent(mementoVME);

    mafString encodeType = "XML";
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafMemento *, mementoVME));
    argList.append(mafEventArgument(mafString, test_file));
    argList.append(mafEventArgument(mafString, encodeType));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.save", mafEventTypeLocal, &argList);

    QVERIFY(mafFile::exists(test_file));
    QFileInfo fInfo3(test_file);
    QVERIFY(fInfo3.size() > 0);


    encodeType = "XML";
    argList.clear();
    argList.append(mafEventArgument(mafString, test_file));
    argList.append(mafEventArgument(mafString, encodeType));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.load", mafEventTypeLocal, &argList);

    mafDEL(mementoVME);

    QDir log_dir(test_dir);
    log_dir.setFilter(QDir::Files);
    mafStringList list = log_dir.entryList();
    int i = 0;

    //remove files crested by test
    for (i; i < list.size(); i++) {
        mafString fileName = test_dir;
        fileName.append("/");
        fileName.append(list.at(i));
        mafFile::remove(fileName);
    }

}

MAF_REGISTER_TEST(mafSerializationExtDataTest);
#include "mafSerializationExtDataTest.moc"
