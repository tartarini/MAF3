/*
 *  mafPipeDataImageThresholdTest.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 16/04/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafEventBusManager.h>
#include <mafPipeDataImageThreshold.h>
#include <mafDataBoundaryAlgorithmVTK.h>
#include <mafResourcesRegistration.h>
#include <mafProxy.h>
#include <mafVME.h>

#include <vtkImageData.h>
#include <vtkImageCanvasSource2D.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;

/**
 Class name: mafPipeDataImageThresholdTest
 This class is the test suite for mafPipeDataImageThreshold.
 */

 //! <title>
//mafPipeDataImageThreshold
//! </title>
//! <description>
//mafPipeDataImageThreshold allows you to make a threshoding on input image data.
//! </description>

class mafPipeDataImageThresholdTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();

        // Create a image data.
        m_ImageCanvas = vtkSmartPointer<vtkImageCanvasSource2D>::New();
        m_ImageCanvas->SetNumberOfScalarComponents(3);
        m_ImageCanvas->SetScalarTypeToUnsignedChar();
        m_ImageCanvas->SetExtent(0, 4, 0, 4, 0, 0);
        m_ImageCanvas->SetDrawColor(3.0, 0, 0);
        m_ImageCanvas->FillBox(0, 4, 0, 4);
        m_ImageCanvas->Update();

        // Create a container with a vtkImageData
        m_ImageData = m_ImageCanvas->GetOutputPort();
        m_ImageData.setClassTypeNameFunction(vtkClassTypeNameExtract);

        // and give it to the mafDataSet.
        //! <snippet>
        m_VME = mafNEW(mafResources::mafVME);
        mafDataSet *dataSet = mafNEW(mafResources::mafDataSet);
        mafDataBoundaryAlgorithmVTK *boundaryAlgorithm;
        boundaryAlgorithm = mafNEW(mafDataBoundaryAlgorithmVTK);
        dataSet->setBoundaryAlgorithm(boundaryAlgorithm);
        dataSet->setDataValue(&m_ImageData);
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

    /// Test setLowerThrehsold function
    void setLowerThrehsoldTest();

    /// Test setUpperThrehsold function
    void setUpperThrehsoldTest();

private:
    mafVME *m_VME; ///< Contain the only item vtkImageData representing the test image.
    mafProxy<vtkAlgorithmOutput> m_ImageData; ///< Container of the vtkImageData
    vtkSmartPointer<vtkImageCanvasSource2D> m_ImageCanvas; ///< Image source.
};

void mafPipeDataImageThresholdTest::updatePipeTest() {
    mafPipeDataImageThreshold *datapipe = mafNEW(mafPluginVTK::mafPipeDataImageThreshold);
    datapipe->setInput(m_VME);
    
    mafVME *output = datapipe->output();
    QVERIFY(output != NULL);

    mafDataSetCollection *collection = output->dataSetCollection();
    mafDataSet *dataSet = collection->itemAtCurrentTime();
    QString dt(dataSet->dataValue()->externalDataType());
    QString res("vtkImageData");

    QCOMPARE(dt, res);

    mafDEL(datapipe);
}

void mafPipeDataImageThresholdTest::setLowerThrehsoldTest() {
    mafPipeDataImageThreshold *datapipe = mafNEW(mafPluginVTK::mafPipeDataImageThreshold);
    datapipe->setLowerThrehsold(5);
    int low = datapipe->lowerThrehsold();
    QVERIFY(low == 5);

    mafDEL(datapipe);
}

void mafPipeDataImageThresholdTest::setUpperThrehsoldTest() {
    mafPipeDataImageThreshold *datapipe = mafNEW(mafPluginVTK::mafPipeDataImageThreshold);
    datapipe->setUpperThrehsold(60);
    int up = datapipe->upperThrehsold();
    QVERIFY(up == 60);

    mafDEL(datapipe);
}


MAF_REGISTER_TEST(mafPipeDataImageThresholdTest);
#include "mafPipeDataImageThresholdTest.moc"
