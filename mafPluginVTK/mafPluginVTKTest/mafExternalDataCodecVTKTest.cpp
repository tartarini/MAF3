/*
 *  mafExternalDataCodecVTKTest.cpp
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

#include <mafExternalDataCodecVTK.h>
#include <mafVME.h>

#include <vtkDataSet.h>
#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkAlgorithmOutput.h>
#include <vtkPolyData.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;


/**
 Class name: mafExternalDataCodecVTKTest
 This class implements the test suite for mafViewVTK.
 */

//! <title>
//mafExternalDataCodecVTK
//! </title>
//! <description>
//Codec to Encode/Decode maf objects into a VTK memory dump.
//! </description>

class mafExternalDataCodecVTKTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        //Create two codec
        m_CodecIn = mafNEW(mafPluginVTK::mafExternalDataCodecVTK);
        m_CodecOut = mafNEW(mafPluginVTK::mafExternalDataCodecVTK);

        // Create a polydata.
        m_DataSource = vtkCubeSource::New();
        m_DataSource->SetXLength(5);
        m_DataSource->SetYLength(3);
        m_DataSource->SetZLength(8);
        m_DataSourceContainer = m_DataSource->GetOutputPort(0);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_CodecIn);
        mafDEL(m_CodecOut);
        m_DataSource->Delete();
    }

    /// mafExternalDataCodecVTK allocation test case.
    void mafCodecVTKAllocationTest();

    /// mafExternalDataCodecBinaryVTK econde test case.
    void mafCodecVTKEncodeDecodeBinaryTest();

    /// mafExternalDataCodecVTK econde test case.
    void mafCodecVTKEncodeDecodeTest();

private:
    mafExternalDataCodecVTK *m_CodecIn; ///< Test var.
    mafExternalDataCodecVTK *m_CodecOut; ///< Test var.
    mafDataStream m_OutputStream; ///< Test var.

    vtkCubeSource *m_DataSource; ///< Source data for the test suite.
    mafContainer<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
};

void mafExternalDataCodecVTKTest::mafCodecVTKAllocationTest() {
    QVERIFY(m_CodecIn != NULL);
    QVERIFY(m_CodecOut != NULL);
}

void mafExternalDataCodecVTKTest::mafCodecVTKEncodeDecodeBinaryTest() {
    vtkPolyDataMapper *sphereMapper = vtkPolyDataMapper::New();

    //Pass data to be encode to the codec
    //! <snippet>
    m_CodecIn->setExternalData(&m_DataSourceContainer);
    //! </snippet>

    //The codec will ecnode the data into the string "outputString"
    //! <snippet>
    char *outputString = m_CodecIn->encode();
    //! </snippet>

    //Decode data encoded
    m_CodecOut->setStringSize(m_CodecIn->stringSize());
    m_CodecOut->decode(outputString);
    mafContainer<vtkAlgorithmOutput> *dataSet = mafContainerPointerTypeCast(vtkAlgorithmOutput, m_CodecOut->externalData());

    sphereMapper->SetInputConnection(*dataSet);

    double boundsIn[6];
    double boundsOut[6];
    m_DataSource->GetOutput()->GetBounds(boundsIn);
    sphereMapper->GetBounds(boundsOut);

    QCOMPARE(boundsIn[0], boundsOut[0]);
    QCOMPARE(boundsIn[1], boundsOut[1]);
    QCOMPARE(boundsIn[2], boundsOut[2]);
    QCOMPARE(boundsIn[3], boundsOut[3]);
    QCOMPARE(boundsIn[4], boundsOut[4]);
    QCOMPARE(boundsIn[5], boundsOut[5]);

    sphereMapper->Delete();
}

void mafExternalDataCodecVTKTest::mafCodecVTKEncodeDecodeTest() {
    vtkPolyDataMapper *sphereMapper = vtkPolyDataMapper::New();

    //Pass data to be encode to the codec
    //! <snippet>
    m_CodecIn->setExternalData(&m_DataSourceContainer);
    //! </snippet>

    //The codec will ecnode the data into the NOT binary string "outputString"
    //! <snippet>
    char *outputString = m_CodecIn->encode(false);
    //! </snippet>

    //Decode data encoded
    m_CodecOut->setStringSize(m_CodecIn->stringSize());
    m_CodecOut->decode(outputString, false);
    mafContainer<vtkAlgorithmOutput> *dataSet = mafContainerPointerTypeCast(vtkAlgorithmOutput, m_CodecOut->externalData());

    sphereMapper->SetInputConnection(*dataSet);

    double boundsIn[6];
    double boundsOut[6];
    m_DataSource->GetOutput()->GetBounds(boundsIn);
    sphereMapper->GetBounds(boundsOut);

    QCOMPARE(boundsIn[0], boundsOut[0]);
    QCOMPARE(boundsIn[1], boundsOut[1]);
    QCOMPARE(boundsIn[2], boundsOut[2]);
    QCOMPARE(boundsIn[3], boundsOut[3]);
    QCOMPARE(boundsIn[4], boundsOut[4]);
    QCOMPARE(boundsIn[5], boundsOut[5]);

    sphereMapper->Delete();
}

MAF_REGISTER_TEST(mafExternalDataCodecVTKTest);
#include "mafExternalDataCodecVTKTest.moc"
