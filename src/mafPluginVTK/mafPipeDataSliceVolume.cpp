/*
 *  mafPipeDataSliceVolume.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 24/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeDataSliceVolume.h"
#include <mafVME.h>
#include <mafDataSet.h>

#include <vtkDataSet.h>
#include <vtkMAFVolumeSlicer.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkExecutive.h>
#include <vtkPointData.h>

#define TEXTURE_RES 512

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafPipeDataSliceVolume::mafPipeDataSliceVolume(const QString code_location) : mafPipeData(code_location), m_Image(NULL) {
    m_SliceOrigin[0] = m_SliceOrigin[1] = m_SliceOrigin[2] = 0.;

    m_Normal[0] = m_Normal[1] = 0.;
    m_Normal[2] = 1.;

    m_SlicePolydata = vtkPolyData::New();

    m_SlicerImage = vtkSmartPointer<vtkMAFVolumeSlicer>::New();
    m_SlicerPolygonal = vtkSmartPointer<vtkMAFVolumeSlicer>::New();
    m_SlicerPolygonal->SetOutput(m_SlicePolydata);

    m_OutputValue = m_SlicerPolygonal->GetOutputPort();
}

mafPipeDataSliceVolume::~mafPipeDataSliceVolume() {
    m_SlicePolydata->Delete();
    if (m_Image) {
        m_Image->Delete();
    }
}

bool mafPipeDataSliceVolume::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        QString dataType = vme->dataSetCollection()->itemAtCurrentTime()->externalDataType();
        if(dataType == "vtkImageData" || dataType == "vtkStructuredPoints" || dataType == "vtkRectilinearGrid") {
            return true;
        }
    }
    return false;
}

void mafPipeDataSliceVolume::updatePipe(double t) {
    /*if (inputList()->size() == 0) {
        return;
    }

    mafDataSet *inputDataSet = dataSetForInput(0, t);
    if(inputDataSet == NULL) {
        return;
    }

    //Get data contained in the mafProxy
    mafProxy<vtkAlgorithmOutput> *volume = mafProxyPointerTypeCast(vtkAlgorithmOutput, inputDataSet->dataValue());
    vtkDataSet *vtk_data = vtkDataSet::SafeDownCast((*volume)->GetProducer()->GetExecutive()->GetOutputData(0));

    m_SlicerImage->SetInputConnection(*volume);
    m_SlicerPolygonal->SetInputConnection(*volume);

    double spc[3] = {.33, .33, 1.};
    if(vtk_data->IsA("vtkImageData") || vtk_data->IsA("vtkStructuredPoints")) {
        ((vtkImageData *)vtk_data)->GetSpacing(spc);
    }

    if (m_Image == NULL) {
        m_Image = vtkImageData::New();
        m_Image->SetScalarType(vtk_data->GetPointData()->GetScalars()->GetDataType());
        m_Image->SetExtent(0, TEXTURE_RES - 1, 0, TEXTURE_RES - 1, 0, 0);
        m_Image->SetSpacing(spc);
        m_SlicerImage->SetOutput(m_Image);
        m_SlicerPolygonal->SetTexture(m_Image);
    }

    m_SlicerImage->SetPlaneOrigin(m_SliceOrigin);
    m_SlicerPolygonal->SetPlaneOrigin(m_SliceOrigin);

    float m_XVector[3] = {1., 0., 0.};
    float m_YVector[3] = {0., 1., 0.};
    m_SlicerImage->SetPlaneAxisX(m_XVector);
    m_SlicerImage->SetPlaneAxisY(m_YVector);
    m_SlicerPolygonal->SetPlaneAxisX(m_XVector);
    m_SlicerPolygonal->SetPlaneAxisY(m_YVector);

    m_SlicerImage->Update();
    m_SlicerPolygonal->Update();

    output(t)->dataSetCollection()->itemAt(t)->setDataValue(&m_OutputValue);
    Superclass::updatePipe(t);*/
}
