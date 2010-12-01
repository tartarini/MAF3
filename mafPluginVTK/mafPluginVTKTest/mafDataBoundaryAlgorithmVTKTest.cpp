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
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();

        // Create a polydata.
        m_DataSource = vtkCubeSource::New();
        m_DataSource->SetXLength(5);
        m_DataSource->SetYLength(3);
        m_DataSource->SetZLength(8);

        m_DataSourceContainer.setExternalCodecType("mafPluginVTK::mafExternalDataCodecVTK");
        m_DataSourceContainer.setExternalDataType("vtkAlgorithmOutput");
        m_DataSourceContainer = m_DataSource->GetOutputPort(0);


        mafPoseMatrix *newMatrix = new mafPoseMatrix();
        newMatrix->set_identity();

        m_DataSetCube = mafNEW(mafResources::mafDataSet);
        m_DataSetCube->setDataValue(&m_DataSourceContainer);
        m_DataSetCube->setPoseMatrix(newMatrix);

    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_DataSetCube);
        m_DataSource->Delete();
    }

    /// Test calculation of VTK boundary.
    void calculateBoundaryTest();


private:
    vtkCubeSource *m_DataSource;
    mafDataSet *m_DataSetCube; ///< DataSet var.
    mafContainer<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
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

    double boundsIn[6] = {-2.5,2.5,-1.5,1.5,-4,4};
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
