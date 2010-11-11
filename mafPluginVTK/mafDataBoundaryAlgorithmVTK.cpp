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

#include <vtkOutlineSource.h>
#include <vtkOutlineCornerFilter.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafDataBoundaryAlgorithmVTK::mafDataBoundaryAlgorithmVTK(const mafString code_location) : mafResources::mafDataBoundaryAlgorithm(code_location) {
}

mafDataBoundaryAlgorithmVTK::~mafDataBoundaryAlgorithmVTK() {
}

mafCore::mafContainerInterface *mafDataBoundaryAlgorithmVTK::calculateBoundary(mafCore::mafContainerInterface *data, mafResources::mafPoseMatrix *matrix) {
    vtkSmartPointer<vtkOutlineSource> box = vtkSmartPointer<vtkOutlineSource>::New();
    vtkSmartPointer<vtkOutlineSource> boxTranformed = vtkSmartPointer<vtkOutlineSource>::New();

    mafContainer<vtkDataSet> *dataSet = mafContainerPointerTypeCast(vtkDataSet, data);

    double b[6];
    (*dataSet)->GetBounds(b);
    box->SetBounds(b);

    //Transform box with the mafPoseMatrix
    vtkTransform *t = vtkTransform::New();
    vtkMatrix4x4 *mat = vtkMatrix4x4::New();
    mat->Identity();
    for(int i=0;i<3;i++)
        mat->SetElement(i,0,matrix->get(i,0));
    for(int i=0;i<3;i++)
        mat->SetElement(i,1,matrix->get(i,1));
    for(int i=0;i<3;i++)
        mat->SetElement(i,2,matrix->get(i,2));
    for(int i=0;i<3;i++)
        mat->SetElement(i,3,matrix->get(i,3));

    t->SetMatrix(mat);
    t->Update();
    vtkTransformPolyDataFilter *ptf = vtkTransformPolyDataFilter::New();
    ptf->SetInput(box->GetOutput());
    ptf->SetTransform(t);
    ptf->Update();
    t->Delete();

    boxTranformed->SetBounds(ptf->GetOutput()->GetBounds());
    boxTranformed->Update();
    ptf->Delete();

    mafCore::mafContainer<vtkOutlineCornerFilter> boundary;
    boundary = vtkOutlineCornerFilter::New();
    boundary->SetInput(boxTranformed->GetOutput());
    boundary->Update();

    return &boundary;
}
