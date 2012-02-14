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
#include <vtkCubeSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkAlgorithmOutput.h>

#include <vtkSmartPointer.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafDataBoundaryAlgorithmVTK::mafDataBoundaryAlgorithmVTK(const QString code_location) : mafResources::mafDataBoundaryAlgorithm(code_location), m_PDataFilter(NULL) {
}

mafDataBoundaryAlgorithmVTK::~mafDataBoundaryAlgorithmVTK() {
    if(m_PDataFilter != NULL) {
        m_PDataFilter->Delete();
    }
}

mafCore::mafProxyInterface *mafDataBoundaryAlgorithmVTK::calculateBoundary(mafCore::mafProxyInterface *data, mafResources::mafMatrix *matrix) {
    if(data) {
        mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, data);
        if (dataSet != NULL) {
            vtkAlgorithm *producer = (*dataSet)->GetProducer();
            vtkDataObject *dataObject = producer->GetOutputDataObject(0);
            vtkDataSet* vtkData = vtkDataSet::SafeDownCast(dataObject);

            double b[6];
            vtkData->GetBounds(b);
            return this->calculateBoundary(b, matrix);
        }
    }
    return NULL;
}

mafCore::mafProxyInterface *mafDataBoundaryAlgorithmVTK::calculateBoundary(double bounds[6], mafResources::mafMatrix *matrix) {
    vtkCubeSource *box = vtkCubeSource::New();
    box->SetBounds(bounds);
    box->Update();

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
        m_PDataFilter->SetInputConnection(box->GetOutputPort(0));
        m_PDataFilter->SetTransform(t);
        m_PDataFilter->Update();
        t->Delete();

        m_OutputBoundary = m_PDataFilter->GetOutputPort(0);
        m_PDataFilter->GetOutput()->GetBounds(m_Bounds);

    } else {
        m_OutputBoundary = box->GetOutputPort(0);
        box->GetOutput()->GetBounds(m_Bounds);
    }
    box->Delete();
    return &m_OutputBoundary;
}
