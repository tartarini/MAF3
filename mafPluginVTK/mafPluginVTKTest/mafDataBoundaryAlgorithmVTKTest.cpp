/*
 *  mafDataBoundaryAlgorithmVTKTest.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 10/11/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafDataBoundaryAlgorithmVTK.h>
#include <mafContainer.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkPoints.h>
#include <vtkFloatArray.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>
#include <vtkAlgorithmOutput.h>
#include <mafPluginManager.h>
#include <mafPlugin.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>

using namespace mafCore;;
using namespace mafResources;
using namespace mafPluginVTK;


/**
 Class name: mafDataBoundaryAlgorithmVTKTest
 This class creates a vtkPolyData and visualizes it trough the mafDataBoundaryAlgorithmVTK
 */

//! <title>
//mafDataBoundaryAlgorithmVTK
//! </title>
//! <description>
//mafDataBoundaryAlgorithmVTK extracts the boundary from the given data value and pose matrix.
//! </description>

class mafDataBoundaryAlgorithmVTKTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafResourcesRegistration::registerResourcesObjects();

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


        mafPoseMatrix *newMatrix = new mafPoseMatrix();
        newMatrix->set_identity();
        newMatrix->put(0,3,5);
        newMatrix->put(1,3,2);
        newMatrix->put(2,3,4);

        m_DataSetCube = mafNEW(mafResources::mafDataSet);
        m_DataSetCube->setDataValue(&m_Container);
        m_DataSetCube->setPoseMatrix(newMatrix);

    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_DataSetCube);
    }

    /// Test calculation of VTK boundary.
    void calculateBoundaryTest();


private:
    mafDataSet *m_DataSetCube; ///< DataSet var.
    mafContainer<vtkPolyData> m_PolyData; ///< Container of the vtkPolyData
    mafContainer<vtkPolyData> m_Container; ///< Test var.
};

void mafDataBoundaryAlgorithmVTKTest::calculateBoundaryTest() {
    mafDataBoundaryAlgorithmVTK *boundaryAlgorithm;
    boundaryAlgorithm = mafNEW(mafDataBoundaryAlgorithmVTK);
    m_DataSetCube->setBoundaryAlgorithm(boundaryAlgorithm);
    mafContainer<vtkAlgorithmOutput> *boundingBox = mafContainerPointerTypeCast(vtkAlgorithmOutput, m_DataSetCube->dataBoundary());


    vtkPolyDataMapper *box = vtkPolyDataMapper::New();
    box->SetInputConnection(*boundingBox);
    box->Update();
    double boundsOut[6];
    box->GetBounds(boundsOut);

    double boundsIn[6] = {5,11,2,8,4,9};
    QCOMPARE(boundsIn[0], boundsOut[0]);
    QCOMPARE(boundsIn[1], boundsOut[1]);
    QCOMPARE(boundsIn[2], boundsOut[2]);
    QCOMPARE(boundsIn[3], boundsOut[3]);
    QCOMPARE(boundsIn[4], boundsOut[4]);
    QCOMPARE(boundsIn[5], boundsOut[5]);
    box->Delete();
}


MAF_REGISTER_TEST(mafDataBoundaryAlgorithmVTKTest);
#include "mafDataBoundaryAlgorithmVTKTest.moc"
