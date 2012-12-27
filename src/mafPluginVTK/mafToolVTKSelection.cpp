/*
 *  mafToolVTKSelection.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 11/11/10.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafToolVTKSelection.h"
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
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafToolVTKSelection::mafToolVTKSelection(const QString code_location) : mafToolVTK(code_location) {
    setObjectName("Tool Selection");

    vtkSmartPointer<vtkCompositeDataPipeline> compositeDataPipeline;

    m_OutlineCornerFilter = vtkOutlineCornerFilter::New();
    m_OutlineCornerFilter->SetExecutive(compositeDataPipeline);

    m_Mapper = vtkPolyDataMapper::New();
    m_Actor = vtkActor::New();
    m_Actor->SetPickable(0);
    m_Actor->SetMapper(m_Mapper);
    m_Actor->GetProperty()->SetLineWidth(3);
    m_Actor->SetVisibility(0);
}

mafToolVTKSelection::~mafToolVTKSelection() {
    m_OutlineCornerFilter->Delete();
    m_Mapper->Delete();
    m_Actor->Delete();
}

bool mafToolVTKSelection::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        QString dataType = vme->dataSetCollection()->itemAtCurrentTime()->externalDataType();
        if(dataType.contains(QRegExp("^vtk.*"))) {
            return true;
        }
    }
    return false;
}

void mafToolVTKSelection::graphicObjectInitialized() {
    // Graphic widget (render window, interactor...) has been created and initialized.
    // now can add the tool's actor.
    addProp(m_Actor, true);
}

void mafToolVTKSelection::resetTool() {
    removeProp(m_Actor);
}

void mafToolVTKSelection::updatePipe(double t) {
    setModified(false);
    mafDataSet *data = dataSetForInput(0, t);
    if(data == NULL) {
        return;
    }

    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, data->dataValue());
    if (dataSet == NULL) {
        m_OutlineCornerFilter->RemoveAllInputs();
    } else {
        vtkAlgorithmOutput *aout = vtkAlgorithmOutput::SafeDownCast(*dataSet);
        if(!aout) {
            return;
        }
        m_OutlineCornerFilter->SetInputConnection(*dataSet);
        m_OutlineCornerFilter->Update();
    }
    m_Mapper->SetInputConnection(0, m_OutlineCornerFilter->GetOutputPort(0));
}
