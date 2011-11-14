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

    m_Thickness = 3.;

    m_CutterPipe = mafNEW(mafPluginVTK::mafPipeDataSliceSurface);
    m_CutterPipe->setParent(this);

    m_Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_Mapper->ScalarVisibilityOff();

    m_Prop3D = vtkActor::New();
    m_Prop3D.setDestructionFunction(&vtkActor::Delete);
    vtkActor::SafeDownCast(m_Prop3D)->SetMapper(m_Mapper);
    m_Output = &m_Prop3D;
}

mafPipeVisualVTKSliceSurface::~mafPipeVisualVTKSliceSurface() {
    mafDEL(m_CutterPipe);
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

    m_CutterPipe->setInput(input());
    m_CutterPipe->updatePipe(t);

    mafVME *output = m_CutterPipe->output();
    mafDataSetCollection *collection = output->dataSetCollection();
    mafDataSet *dataSet = collection->itemAt(t);
    mafProxy<vtkAlgorithmOutput> *data = mafProxyPointerTypeCast(vtkAlgorithmOutput, dataSet->dataValue());
    m_Mapper->SetInputConnection(*data);

    vtkActor *actor = vtkActor::SafeDownCast(m_Prop3D);
    actor->GetProperty()->SetLineWidth(m_Thickness);
}

void mafPipeVisualVTKSliceSurface::setSlice(double *origin) {
    m_CutterPipe->setSliceOrigin(origin);
}

void mafPipeVisualVTKSliceSurface::setNormal(double *normal) {
    m_CutterPipe->setNormal(normal);
}
