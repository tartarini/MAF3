/*
 *  mafDataPipeImageThresholdTest.cpp
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
#include <mafDataPipeImageThreshold.h>
#include <mafResourcesRegistration.h>
#include <mafContainer.h>
#include <mafVME.h>

#include <vtkImageData.h>
#include <vtkImageCanvasSource2D.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;

/**
 Class name: mafDataPipeImageThresholdTest
 This class is the test suite for mafDataPipeImageThreshold.
 */

 //! <title>
//mafDataPipeImageThreshold
//! </title>
//! <description>
//mafDataPipeImageThreshold allows you to make a threshoding on input image data.
//! </description>

class mafDataPipeImageThresholdTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
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
        m_ImageData = m_ImageCanvas->GetOutput();

        // and give it to the mafDataSet.
        //! <snippet>
        m_VME = mafNEW(mafResources::mafVME);
        mafDataSet *dataSet = mafNEW(mafResources::mafDataSet);
        dataSet->setDataValue(&m_ImageData);
        m_VME->dataSetCollection()->insertItem(dataSet);
        //! </snippet>

    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_VME);
        mafEventBusManager::instance()->shutdown();
    }

    /// Test the creation of the vtkActor
    void updatePipeTest();

    /// Test setLowerThrehsold function
    void setLowerThrehsoldTest();

    /// Test setUpperThrehsold function
    void setUpperThrehsoldTest();

private:
    mafVME *m_VME; ///< Contain the only item vtkImageData representing the test image.
    mafContainer<vtkImageData> m_ImageData; ///< Container of the vtkImageData
    vtkSmartPointer<vtkImageCanvasSource2D> m_ImageCanvas; ///< Image source.
};

void mafDataPipeImageThresholdTest::updatePipeTest() {
    mafDataPipeImageThreshold *datapipe = mafNEW(mafPluginVTK::mafDataPipeImageThreshold);
    datapipe->setInput(m_VME);
    datapipe->createPipe();
    datapipe->updatePipe();

    mafVME *output = datapipe->output();
    QVERIFY(output != NULL);

    mafDataSetCollection *collection = output->dataSetCollection();
    mafDataSet *dataSet = collection->itemAtCurrentTime();
    mafContainer<vtkImageData> *image = mafContainerPointerTypeCast(vtkImageData, dataSet->dataValue());
    mafString dt(image->externalDataType());
    mafString res("vtkImageData");

    QCOMPARE(dt, res);

    mafDEL(datapipe);
}

void mafDataPipeImageThresholdTest::setLowerThrehsoldTest() {
    mafDataPipeImageThreshold *datapipe = mafNEW(mafPluginVTK::mafDataPipeImageThreshold);
    datapipe->setLowerThrehsold(5);
    int low = datapipe->lowerThrehsold();
    QVERIFY(low == 5);

    mafDEL(datapipe);
}

void mafDataPipeImageThresholdTest::setUpperThrehsoldTest() {
    mafDataPipeImageThreshold *datapipe = mafNEW(mafPluginVTK::mafDataPipeImageThreshold);
    datapipe->setUpperThrehsold(60);
    int up = datapipe->upperThrehsold();
    QVERIFY(up == 60);

    mafDEL(datapipe);
}


MAF_REGISTER_TEST(mafDataPipeImageThresholdTest);
#include "mafDataPipeImageThresholdTest.moc"
