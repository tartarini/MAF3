/*
 *  mafPipeVisualSelection.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 11/11/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisualSelection.h"

#include <vtkSmartPointer.h>
#include <vtkAlgorithmOutput.h>
#include <vtkOutlineCornerFilter.h>
#include <vtkCompositeDataPipeline.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafPipeVisualSelection::mafPipeVisualSelection(const QString code_location) : mafPipeVisual(code_location) {
}

mafPipeVisualSelection::~mafPipeVisualSelection() {
}

bool mafPipeVisualSelection::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        QString dataType = vme->dataSetCollection()->itemAtCurrentTime()->externalDataType();
        if(dataType.startsWith("vtkAlgorithmOutput", Qt::CaseSensitive)) {
            return true;
        }
    }
    return false;
}

void mafPipeVisualSelection::createPipe() {
    mafVME *inputVME = this->inputList()->at(0);
    mafDataSet *data = inputVME->dataSetCollection()->itemAtCurrentTime();
    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, data->dataValue());

    vtkSmartPointer<vtkCompositeDataPipeline> compositeDataPipeline;

    m_OutlineCornerFilter = vtkOutlineCornerFilter::New();
    m_OutlineCornerFilter->SetExecutive(compositeDataPipeline);
    m_OutlineCornerFilter->SetInputConnection(*dataSet);

    vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(0, m_OutlineCornerFilter->GetOutputPort(0));
    m_Actor = vtkActor::New();
    m_Actor.setDestructionFunction(&vtkActor::Delete);
    m_Actor->SetMapper(mapper);
    mapper->Delete();
    m_Output = &m_Actor;
}

void mafPipeVisualSelection::updatePipe(double t) {
    mafVMEList *inputList = this->inputList();
    mafDataSet *data = inputList->at(0)->dataSetCollection()->itemAt(t);
    if(data == NULL) {
        return;
    }

    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, data->dataValue());
    m_OutlineCornerFilter->SetInputConnection(*dataSet);
    m_OutlineCornerFilter->Update();
}
