/*
 *  mafPipeVisualVTKSelection.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 11/11/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisualVTKSelection.h"
#include "mafVTKWidget.h"
#include <mafVME.h>
#include <mafDataSet.h>

#include <vtkSmartPointer.h>
#include <vtkAlgorithmOutput.h>
#include <vtkOutlineCornerFilter.h>
#include <vtkCompositeDataPipeline.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafPipeVisualVTKSelection::mafPipeVisualVTKSelection(const QString code_location) : mafPipeVisualVTK(code_location) {
    vtkSmartPointer<vtkCompositeDataPipeline> compositeDataPipeline;

    m_OutlineCornerFilter = vtkOutlineCornerFilter::New();
    m_OutlineCornerFilter->SetExecutive(compositeDataPipeline);

    m_Mapper = vtkPolyDataMapper::New();
    m_Prop3D = vtkActor::New();
    m_Prop3D->SetPickable(0);
    m_Prop3D.setDestructionFunction(&vtkActor::Delete);
    vtkActor::SafeDownCast(m_Prop3D)->SetMapper(m_Mapper);
    vtkActor::SafeDownCast(m_Prop3D)->GetProperty()->SetLineWidth(3);
    m_Output = &m_Prop3D;
}

mafPipeVisualVTKSelection::~mafPipeVisualVTKSelection() {
    m_OutlineCornerFilter->Delete();
    m_Mapper->Delete();
}

bool mafPipeVisualVTKSelection::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        QString dataType = vme->dataSetCollection()->itemAtCurrentTime()->externalDataType();
        if(dataType.contains(QRegExp("^vtk.*"))) {
            return true;
        }
    }
    return false;
}

void mafPipeVisualVTKSelection::updatePipe(double t) {
    setModified(false);
    mafDataSet *data = dataSetForInput(0, t);
    if(data == NULL) {
        return;
    }

    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, data->dataValue());
    if (dataSet == NULL) {
        m_OutlineCornerFilter->RemoveAllInputs();
    } else {
        m_OutlineCornerFilter->SetInputConnection(*dataSet);
        m_OutlineCornerFilter->Update();
    }
    m_Mapper->SetInputConnection(0, m_OutlineCornerFilter->GetOutputPort(0));
}
