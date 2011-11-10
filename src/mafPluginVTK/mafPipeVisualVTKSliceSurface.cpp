/*
 *  mafPipeVisualVTKSliceSurface.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 09/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisualVTKSliceSurface.h"

#include <mafVME.h>
#include <mafDataSet.h>
#include <mafDataSetCollection.h>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkPlane.h>
#include <vtkCutter.h>

#include <vtkAlgorithm.h>
#include <vtkAlgorithmOutput.h>
#include <vtkDataObject.h>
#include <vtkDataSet.h>
#include <vtkProperty.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;


mafPipeVisualVTKSliceSurface::mafPipeVisualVTKSliceSurface(const QString code_location) : mafPipeVisualVTK(code_location) {
//    m_UIFilename = "mafPipeVisualVTKSliceSurface.ui";

    m_SliceOrigin[0] = m_SliceOrigin[1] = m_SliceOrigin[2] = 0.;
    
    m_Normal[0] = m_Normal[1] = 0.;
    m_Normal[2] = 1.;

    m_Thickness = 3.;

    m_Plane = vtkPlane::New();

    m_Cutter = vtkCutter::New();
    m_Cutter->SetCutFunction(m_Plane);
    
    vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection( m_Cutter->GetOutputPort());
    mapper->ScalarVisibilityOff();

    m_Prop3D = vtkActor::New();
    m_Prop3D.setDestructionFunction(&vtkActor::Delete);
    vtkActor::SafeDownCast(m_Prop3D)->SetMapper(mapper);
    m_Output = &m_Prop3D;
    mapper->Delete();
}

mafPipeVisualVTKSliceSurface::~mafPipeVisualVTKSliceSurface() {
    m_Plane->Delete();
    m_Cutter->Delete();
}

bool mafPipeVisualVTKSliceSurface::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        QString dataType = vme->dataSetCollection()->itemAtCurrentTime()->externalDataType();
        if(dataType.startsWith("vtkPolyData", Qt::CaseSensitive)) {
            return true;
        }
    }
    return false;
}

void mafPipeVisualVTKSliceSurface::updatePipe(double t) {
    Superclass::updatePipe(t);

    mafDataSet *data = dataSetForInput(0, t);
    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, data->dataValue());

    m_Plane->SetOrigin(m_SliceOrigin);
    m_Plane->SetNormal(m_Normal);

    m_Cutter->SetInputConnection(*dataSet);
    m_Cutter->Update();

    vtkActor *actor = vtkActor::SafeDownCast(m_Prop3D);
    actor->GetProperty()->SetLineWidth(m_Thickness);
}

void mafPipeVisualVTKSliceSurface::SetSlice(double *origin) {
    m_SliceOrigin[0] = origin[0];
    m_SliceOrigin[1] = origin[1];
    m_SliceOrigin[2] = origin[2];
}

void mafPipeVisualVTKSliceSurface::SetNormal(double *normal) {
    m_Normal[0] = normal[0];
    m_Normal[1] = normal[1];
    m_Normal[2] = normal[2];
}
