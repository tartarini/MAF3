/*
 *  mafDataBoundaryAlgorithmVTK.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 10/11/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafDataBoundaryAlgorithmVTK.h"
#include <mafContainerInterface.h>
#include <vtkSmartPointer.h>
#include <vtkDataSetReader.h>
#include <vtkDataSetWriter.h>
#include <vtkDataSet.h>
#include <vtkPolyData.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkAlgorithmOutput.h>

#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafDataBoundaryAlgorithmVTK::mafDataBoundaryAlgorithmVTK(const mafString code_location) : mafResources::mafDataBoundaryAlgorithm(code_location), m_Box(NULL), m_PDataFilter(NULL) {
}

mafDataBoundaryAlgorithmVTK::~mafDataBoundaryAlgorithmVTK() {
    if(m_Box != NULL) {
        m_Box->Delete();
    }
    if(m_PDataFilter != NULL) {
        m_PDataFilter->Delete();
    }
}

mafCore::mafContainerInterface *mafDataBoundaryAlgorithmVTK::calculateBoundary(mafCore::mafContainerInterface *data, mafResources::mafPoseMatrix *matrix) {
    mafContainer<vtkAlgorithmOutput> *dataSet = mafContainerPointerTypeCast(vtkAlgorithmOutput, data);

    vtkSmartPointer<vtkDataSetMapper> box = vtkSmartPointer<vtkDataSetMapper>::New();
    box->SetInputConnection(*dataSet);
    box->Update();
    double b[6];
    box->GetBounds(b);
    return this->calculateBoundary(b, matrix);
}

mafCore::mafContainerInterface *mafDataBoundaryAlgorithmVTK::calculateBoundary(double bounds[6], mafResources::mafPoseMatrix *matrix) {
    m_Box = vtkCubeSource::New();
    m_Box->SetBounds(bounds);
    m_Box->Update();

    if(matrix != NULL){
        //Transform box with the mafPoseMatrix
        vtkTransform *t = vtkTransform::New();
        vtkMatrix4x4 *mat = vtkMatrix4x4::New();
        mat->Identity();
        for(int i=0;i<3;++i)
            mat->SetElement(i,0,matrix->get(i,0));
        for(int i=0;i<3;++i)
            mat->SetElement(i,1,matrix->get(i,1));
        for(int i=0;i<3;++i)
            mat->SetElement(i,2,matrix->get(i,2));
        for(int i=0;i<3;++i)
            mat->SetElement(i,3,matrix->get(i,3));

        t->SetMatrix(mat);
        t->Update();
        m_PDataFilter = vtkTransformPolyDataFilter::New();
        m_PDataFilter->SetInputConnection(m_Box->GetOutputPort(0));
        m_PDataFilter->SetTransform(t);
        m_PDataFilter->Update();
        t->Delete();

        m_OutputBoundary = m_PDataFilter->GetOutputPort(0);
        m_PDataFilter->GetOutput()->GetBounds(m_Bounds);

    } else {
        m_OutputBoundary = m_Box->GetOutputPort(0);
        m_Box->GetOutput()->GetBounds(m_Bounds);
    }
    return &m_OutputBoundary;
}
void mafDataBoundaryAlgorithmVTK::bounds(double bounds[6]) {
    int i = 0;
    for(i; i < 6; ++i){
        bounds[i] = m_Bounds[i];
    }
}

