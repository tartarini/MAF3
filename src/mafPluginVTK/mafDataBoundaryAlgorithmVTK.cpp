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
#include <mafProxyInterface.h>
#include <vtkSmartPointer.h>
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

mafDataBoundaryAlgorithmVTK::mafDataBoundaryAlgorithmVTK(const QString code_location) : mafResources::mafDataBoundaryAlgorithm(code_location), m_Box(NULL), m_PDataFilter(NULL) {
}

mafDataBoundaryAlgorithmVTK::~mafDataBoundaryAlgorithmVTK() {
    if(m_Box != NULL) {
        m_Box->Delete();
    }
    if(m_PDataFilter != NULL) {
        m_PDataFilter->Delete();
    }
}

mafCore::mafProxyInterface *mafDataBoundaryAlgorithmVTK::calculateBoundary(mafCore::mafProxyInterface *data, mafResources::mafMatrix *matrix) {
    if(data) {
        mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, data);
        vtkSmartPointer<vtkDataSetMapper> box = vtkSmartPointer<vtkDataSetMapper>::New();
        box->SetInputConnection(*dataSet);
        box->Update();
        double b[6];
        box->GetBounds(b);
        return this->calculateBoundary(b, matrix);
    }
    return NULL;
}

mafCore::mafProxyInterface *mafDataBoundaryAlgorithmVTK::calculateBoundary(double bounds[6], mafResources::mafMatrix *matrix) {
    m_Box = vtkCubeSource::New();
    m_Box->SetBounds(bounds);
    m_Box->Update();

    if(matrix != NULL){
        //Transform box with the mafMatrix
        vtkTransform *t = vtkTransform::New();
        vtkMatrix4x4 *mat = vtkMatrix4x4::New();
        mat->Identity();
        for(int i=0;i<3;++i)
            mat->SetElement(i,0,matrix->element(i,0));
        for(int i=0;i<3;++i)
            mat->SetElement(i,1,matrix->element(i,1));
        for(int i=0;i<3;++i)
            mat->SetElement(i,2,matrix->element(i,2));
        for(int i=0;i<3;++i)
            mat->SetElement(i,3,matrix->element(i,3));

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
