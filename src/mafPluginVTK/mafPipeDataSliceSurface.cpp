/*
 *  mafPipeDataSliceSurface.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 14/11/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeDataSliceSurface.h"
#include <mafVME.h>
#include <mafDataSet.h>

#include <vtkDataSet.h>
#include <vtkPlane.h>
#include <vtkCutter.h>


using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafPipeDataSliceSurface::mafPipeDataSliceSurface(const QString code_location) : mafPipeDataSlice(code_location) {
    m_Plane = vtkSmartPointer<vtkPlane>::New();
    m_Cutter = vtkSmartPointer<vtkCutter>::New();
    m_Cutter->SetCutFunction(m_Plane);

    m_OutputValue = m_Cutter->GetOutputPort();
}

mafPipeDataSliceSurface::~mafPipeDataSliceSurface() {
}

bool mafPipeDataSliceSurface::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        QString dataType = vme->dataSetCollection()->itemAtCurrentTime()->externalDataType();
        if(dataType == "vtkPolyData") {
            return true;
        }
    }
    return false;
}

void mafPipeDataSliceSurface::updatePipe(double t) {
    if (inputList()->size() == 0) {
        return;
    }

    mafDataSet *inputDataSet = dataSetForInput(0, t);
    if(inputDataSet == NULL) {
        return;
    }

    m_Plane->SetOrigin(sliceOrigin());
    m_Plane->SetNormal(normal());
    m_Plane->SetTransform(m_TransformMatrix.rawData());
    m_Plane->Modified();

    //Get data contained in the mafProxy
    mafProxy<vtkAlgorithmOutput> *surface = mafProxyPointerTypeCast(vtkAlgorithmOutput, inputDataSet->dataValue());

    m_Cutter->SetInputConnection(*surface);
    m_Cutter->Update();

    output(t)->dataSetCollection()->itemAt(t)->setDataValue(&m_OutputValue);
    Superclass::updatePipe(t);
}
