/*
 *  mafPipeDataSliceVolumeTest.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 24/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestConfig.h>
#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafEventBusManager.h>
#include <mafPipeDataSliceVolume.h>
#include <mafDataBoundaryAlgorithmVTK.h>
#include <mafResourcesRegistration.h>
#include <mafProxy.h>
#include <mafVME.h>
#include <mafDataSet.h>

#include <vtkDataSetReader.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;

/**
 Class name: mafPipeDataSliceVolumeTest
 This class is the test suite for mafPipeDataSliceVolume.
 */

 //! <title>
//mafPipeDataSliceVolume
//! </title>
//! <description>
//mafPipeDataSliceVolume allows you to make a slice on input volume data.
//! </description>

class mafPipeDataSliceVolumeTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();

        QString fname(MAF_DATA_DIR);
        fname.append("/VTK/mafPipeVisualVTKIsoSurfaceTestData.vtk");

        // Import a VTK volume.
        vtkDataSetReader *reader = vtkDataSetReader::New();
        fname = QDir::toNativeSeparators(fname);
        QByteArray ba = fname.toAscii();
        reader->SetFileName(ba.data());
        reader->Update();

        // Create a container with a vtkImageData
        m_Volume = reader->GetOutputPort();
        m_Volume.setClassTypeNameFunction(vtkClassTypeNameExtract);

        // and give it to the mafDataSet.
        //! <snippet>
        m_VME = mafNEW(mafResources::mafVME);
        mafDataSet *dataSet = mafNEW(mafResources::mafDataSet);
        mafDataBoundaryAlgorithmVTK *boundaryAlgorithm;
        boundaryAlgorithm = mafNEW(mafDataBoundaryAlgorithmVTK);
        dataSet->setBoundaryAlgorithm(boundaryAlgorithm);
        dataSet->setDataValue(&m_Volume);
        m_VME->dataSetCollection()->insertItem(dataSet);
        mafDEL(dataSet);
        reader->Delete();
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
    mafVME *m_VME; ///< Contains the test volume data.
    mafProxy<vtkAlgorithmOutput> m_Volume; ///< Container of VTK volume data.
};

void mafPipeDataSliceVolumeTest::updatePipeTest() {
    mafPipeDataSliceVolume *datapipe = mafNEW(mafPluginVTK::mafPipeDataSliceVolume);
    datapipe->setInput(m_VME);
    
    mafVME *output = datapipe->output();
    QVERIFY(output != NULL);

    datapipe->updatePipe();
    mafDataSetCollection *collection = output->dataSetCollection();
    mafDataSet *dataSet = collection->itemAtCurrentTime();
    QString dt(dataSet->dataValue()->externalDataType());
    QString res("vtkAlgorithmOutput");

    QCOMPARE(dt, res);

    mafProxyInterfacePointer texture = datapipe->textureOutput();
    QVERIFY(texture != NULL);

    mafDEL(datapipe);
}

void mafPipeDataSliceVolumeTest::setSliceTest() {
    mafPipeDataSliceVolume *datapipe = mafNEW(mafPluginVTK::mafPipeDataSliceVolume);
	double s[3] = {10, 35, -1};
    datapipe->setSliceOrigin(s);
    double *sp = datapipe->sliceOrigin();
    QVERIFY(sp[0] == 10);
	QVERIFY(sp[1] == 35);
	QVERIFY(sp[2] == -1);

    mafDEL(datapipe);
}

void mafPipeDataSliceVolumeTest::setNormalTest() {
    mafPipeDataSliceVolume *datapipe = mafNEW(mafPluginVTK::mafPipeDataSliceVolume);
	double n[3] = {1,0,0};
    datapipe->setNormal(n);
    double *np = datapipe->normal();
    QVERIFY(np[0] == 1);
	QVERIFY(np[1] == 0);
	QVERIFY(np[2] == 0);

    mafDEL(datapipe);
}

MAF_REGISTER_TEST(mafPipeDataSliceVolumeTest);
#include "mafPipeDataSliceVolumeTest.moc"
