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

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafPipeDataSliceVolume::mafPipeDataSliceVolume(const QString code_location) : mafPipeData(code_location) {
    m_SliceOrigin[0] = m_SliceOrigin[1] = m_SliceOrigin[2] = 0.;

    m_Normal[0] = m_Normal[1] = 0.;
    m_Normal[2] = 1.;

    m_Slicer = vtkSmartPointer<vtkMAFVolumeSlicer>::New();

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

    m_Slicer->SetInputConnection(*volume);
    m_Slicer->SetPlaneOrigin(m_SliceOrigin);

    float m_XVector[3] = {1., 0., 0.};
    float m_YVector[3] = {0., 1., 0.};
    m_Slicer->SetPlaneAxisX(m_XVector);
    m_Slicer->SetPlaneAxisY(m_YVector);

    m_Slicer->Update();

    output(t)->dataSetCollection()->itemAt(t)->setDataValue(&m_OutputPolygonal);
    Superclass::updatePipe(t);
}
