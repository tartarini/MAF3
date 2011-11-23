/*
 *  mafPipeDataSliceVolume.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 14/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeDataSliceVolume.h"
#include <mafVME.h>
#include <mafDataSet.h>

#include <vtkDataSet.h>
#include <vtkPlane.h>
#include <vtkCutter.h>


using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafPipeDataSliceVolume::mafPipeDataSliceVolume(const QString code_location) : mafPipeData(code_location) {
    m_SliceOrigin[0] = m_SliceOrigin[1] = m_SliceOrigin[2] = 0.;

    m_Normal[0] = m_Normal[1] = 0.;
    m_Normal[2] = 1.;

    m_Plane = vtkSmartPointer<vtkPlane>::New();
    m_Cutter = vtkSmartPointer<vtkCutter>::New();
    m_Cutter->SetCutFunction(m_Plane);

    m_OutputValue = m_Cutter->GetOutputPort();
}

mafPipeDataSliceVolume::~mafPipeDataSliceVolume() {
}

bool mafPipeDataSliceVolume::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        QString dataType = vme->dataSetCollection()->itemAtCurrentTime()->externalDataType();
        if(dataType == "vtkPolyData") {
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

    m_Plane->SetOrigin(m_SliceOrigin);
    m_Plane->SetNormal(m_Normal);

    //Get data contained in the mafProxy
    mafProxy<vtkAlgorithmOutput> *surface = mafProxyPointerTypeCast(vtkAlgorithmOutput, inputDataSet->dataValue());

    m_Cutter->SetInputConnection(*surface);
    m_Cutter->Update();

    output(t)->dataSetCollection()->itemAt(t)->setDataValue(&m_OutputValue);
    Superclass::updatePipe(t);
}
