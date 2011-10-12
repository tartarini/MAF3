/*
 *  mafPipeVisualVTKBox.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 13/07/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisualVTKBox.h"
#include "mafVTKWidget.h"
#include <mafVME.h>
#include <mafDataSet.h>

#include <vtkSmartPointer.h>
#include <vtkAlgorithmOutput.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafPipeVisualVTKBox::mafPipeVisualVTKBox(const QString code_location) : mafPipeVisualVTK(code_location) {
    m_OutlineFilter = vtkOutlineFilter::New();

    vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(0, m_OutlineFilter->GetOutputPort(0));
    m_Prop3D = vtkActor::New();
    m_Prop3D.setDestructionFunction(&vtkActor::Delete);
    vtkActor::SafeDownCast(m_Prop3D)->SetMapper(mapper);
    mapper->Delete();
    m_Output = &m_Prop3D;
}

mafPipeVisualVTKBox::~mafPipeVisualVTKBox() {
}

bool mafPipeVisualVTKBox::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        QString dataType = vme->dataSetCollection()->itemAtCurrentTime()->externalDataType();
        if(dataType.contains(QRegExp("^vtk.*"))) {
            return true;
        }
    }
    return false;
}

void mafPipeVisualVTKBox::updatePipe(double t) {
    setModified(false);
    mafDataSet *data = dataSetForInput(0, t);
    if(data == NULL) {
        return;
    }

    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, data->dataValue());
    if (dataSet == NULL) {
        m_OutlineFilter->RemoveAllInputs();
    } else {
        m_OutlineFilter->SetInputConnection((*dataSet));
        m_OutlineFilter->Update();
    }
}
