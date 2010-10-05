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
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkFloatArray.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>


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
        //Create two codec
        m_CodecIn = mafNEW(mafPluginVTK::mafExternalDataCodecVTK);
        m_CodecOut = mafNEW(mafPluginVTK::mafExternalDataCodecVTK);

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
        m_Container = surface;
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_CodecIn);
        mafDEL(m_CodecOut);
        mafCoreSingletons::mafSingletonsShutdown();
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
    mafContainer<vtkPolyData> m_Container; ///< Test var.
    mafDataStream m_OutputStream; ///< Test var.
};

void mafExternalDataCodecVTKTest::mafCodecVTKAllocationTest() {
    QVERIFY(m_CodecIn != NULL);
    QVERIFY(m_CodecOut != NULL);
}

void mafExternalDataCodecVTKTest::mafCodecVTKEncodeDecodeBinaryTest() {
    //Pass data to be encode to the codec
    //! <snippet>
    m_CodecIn->setExternalData(&m_Container);
    //! </snippet>

    //The codec will ecnode the data into the string "outputString"
    //! <snippet>
    char *outputString = m_CodecIn->encode();
    //! </snippet>

    //Decode data encoded
    m_CodecOut->setStringSize(m_CodecIn->stringSize());
    m_CodecOut->decode(outputString);
    mafContainer<vtkPolyData> *dataSet = mafContainerPointerTypeCast(vtkPolyData, m_CodecOut->externalData());

    double boundsIn[6];
    double boundsOut[6];
    m_Container->GetBounds(boundsIn);
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
}

void mafExternalDataCodecVTKTest::mafCodecVTKEncodeDecodeTest() {
    //Pass data to be encode to the codec
    //! <snippet>
    m_CodecIn->setExternalData(&m_Container);
    //! </snippet>

    //The codec will ecnode the data into the NOT binary string "outputString"
    //! <snippet>
    char *outputString = m_CodecIn->encode(false);
    //! </snippet>

    //Decode data encoded
    m_CodecOut->setStringSize(m_CodecIn->stringSize());
    m_CodecOut->decode(outputString, false);
    mafContainer<vtkPolyData> *dataSet = mafContainerPointerTypeCast(vtkPolyData, m_CodecOut->externalData());

    double boundsIn[6];
    double boundsOut[6];
    m_Container->GetBounds(boundsIn);
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
}

MAF_REGISTER_TEST(mafExternalDataCodecVTKTest);
#include "mafExternalDataCodecVTKTest.moc"
