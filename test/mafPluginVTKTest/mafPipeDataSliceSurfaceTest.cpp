/*
 *  mafPipeDataSliceSurfaceTest.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 14/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafEventBusManager.h>
#include <mafPipeDataSliceSurface.h>
#include <mafDataBoundaryAlgorithmVTK.h>
#include <mafResourcesRegistration.h>
#include <mafProxy.h>
#include <mafVME.h>
#include <mafDataSet.h>

#include <vtkPolyData.h>
#include <vtkSphereSource.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;

/**
 Class name: mafPipeDataSliceSurfaceTest
 This class is the test suite for mafPipeDataSliceSurface.
 */

 //! <title>
//mafPipeDataSliceSurface
//! </title>
//! <description>
//mafPipeDataSliceSurface allows you to make a slice on input surface data.
//! </description>

class mafPipeDataSliceSurfaceTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();

        // Create a surface data.
        m_Sphere = vtkSmartPointer<vtkSphereSource>::New();
        m_Sphere->SetRadius(3);
        m_Sphere->Update();

        // Create a container with a vtkImageData
        m_PolyData = m_Sphere->GetOutputPort();
        m_PolyData.setClassTypeNameFunction(vtkClassTypeNameExtract);

        // and give it to the mafDataSet.
        //! <snippet>
        m_VME = mafNEW(mafResources::mafVME);
        mafDataSet *dataSet = mafNEW(mafResources::mafDataSet);
        mafDataBoundaryAlgorithmVTK *boundaryAlgorithm;
        boundaryAlgorithm = mafNEW(mafDataBoundaryAlgorithmVTK);
        dataSet->setBoundaryAlgorithm(boundaryAlgorithm);
        dataSet->setDataValue(&m_PolyData);
        m_VME->dataSetCollection()->insertItem(dataSet);
        mafDEL(dataSet);
        //! </snippet>
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_VME);
        mafEventBusManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// Test the creation of the vtkActor
    void updatePipeTest();

    /// Test setSlice function
    void setSliceTest();

    /// Test setNormal function
    void setNormalTest();

private:
    mafVME *m_VME; ///< Contain the vtkPolyData representing the test surface.
    mafProxy<vtkAlgorithmOutput> m_PolyData; ///< Container of the vtkPolyData.
    vtkSmartPointer<vtkSphereSource> m_Sphere; ///< Sphere source.
};

void mafPipeDataSliceSurfaceTest::updatePipeTest() {
    mafPipeDataSliceSurface *datapipe = mafNEW(mafPluginVTK::mafPipeDataSliceSurface);
    datapipe->setInput(m_VME);
    
    mafVME *output = datapipe->output();
    QVERIFY(output != NULL);

    datapipe->updatePipe();
    mafDataSetCollection *collection = output->dataSetCollection();
    mafDataSet *dataSet = collection->itemAtCurrentTime();
    QString dt(dataSet->dataValue()->externalDataType());
    QString res("vtkAlgorithmOutput");

    QCOMPARE(dt, res);

    mafDEL(datapipe);
}

void mafPipeDataSliceSurfaceTest::setSliceTest() {
    mafPipeDataSliceSurface *datapipe = mafNEW(mafPluginVTK::mafPipeDataSliceSurface);
	double s[3] = {10, 35, -1};
    datapipe->setSliceOrigin(s);
    double *sp = datapipe->sliceOrigin();
    QVERIFY(sp[0] == 10);
	QVERIFY(sp[1] == 35);
	QVERIFY(sp[2] == -1);

    mafDEL(datapipe);
}

void mafPipeDataSliceSurfaceTest::setNormalTest() {
    mafPipeDataSliceSurface *datapipe = mafNEW(mafPluginVTK::mafPipeDataSliceSurface);
	double n[3] = {1,0,0};
    datapipe->setNormal(n);
    double *np = datapipe->normal();
    QVERIFY(np[0] == 1);
	QVERIFY(np[1] == 0);
	QVERIFY(np[2] == 0);

    mafDEL(datapipe);
}


MAF_REGISTER_TEST(mafPipeDataSliceSurfaceTest);
#include "mafPipeDataSliceSurfaceTest.moc"
