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
#include <vtkMatrixToLinearTransform.h>
#include <vtkMatrix4x4.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafPipeDataSliceVolume::mafPipeDataSliceVolume(const QString code_location) : mafPipeDataSlice(code_location) {
    m_Slicer = vtkSmartPointer<vtkMAFVolumeSlicer>::New();
    m_Transform = vtkSmartPointer<vtkMatrixToLinearTransform>::New();
    m_Slicer->SetSliceTransform(m_Transform);

    m_OutputPolygonal = m_Slicer->GetOutputPort();
    m_OutputTexture   = m_Slicer->GetTexturedOutputPort();
}

mafPipeDataSliceVolume::~mafPipeDataSliceVolume() {
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
    if (inputList()->size() == 0) {
        return;
    }

    mafDataSet *inputDataSet = dataSetForInput(0, t);
    if(inputDataSet == NULL) {
        return;
    }

    //Get data contained in the mafProxy
    mafProxy<vtkAlgorithmOutput> *volume = mafProxyPointerTypeCast(vtkAlgorithmOutput, inputDataSet->dataValue());

    mafMatrix *matrix = inputDataSet->poseMatrix();

    vtkMatrix4x4 *mat = vtkMatrix4x4::New();
    mat->DeepCopy(matrix->rawData());
    m_Transform->SetInput(mat);
    mat->Delete();

    m_Slicer->SetInputConnection(*volume);
    m_Slicer->SetPlaneOrigin(sliceOrigin());

    m_Slicer->SetPlaneAxisX(xVector());
    m_Slicer->SetPlaneAxisY(yVector());
    m_Slicer->SetSliceTransform(m_Transform);

    m_Slicer->Update();

    output(t)->dataSetCollection()->itemAt(t)->setDataValue(&m_OutputPolygonal);
    Superclass::updatePipe(t);
}
