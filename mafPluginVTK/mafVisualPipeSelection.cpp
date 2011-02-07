/*
 *  mafVisualPipeSelection.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 11/11/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVisualPipeSelection.h"

#include <vtkSmartPointer.h>
#include <vtkAlgorithmOutput.h>
#include <vtkOutlineCornerFilter.h>
#include <vtkCompositeDataPipeline.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafVisualPipeSelection::mafVisualPipeSelection(const mafString code_location) : mafVisualPipe(code_location) {
}

mafVisualPipeSelection::~mafVisualPipeSelection() {
}

bool mafVisualPipeSelection::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = dynamic_cast<mafVME*>(obj);
    if(vme != NULL) {
        mafCore::mafContainerInterface *data = vme->dataSetCollection()->itemAtCurrentTime()->dataValue();
        if(data != NULL) {
            mafString dataType = data->externalDataType();
            if(dataType.startsWith("vtkAlgorithmOutput", Qt::CaseSensitive)) {
                return true;
            }
        }
    }
    return false;
}

void mafVisualPipeSelection::createPipe() {
    mafVME *inputVME = this->inputList()->at(0);
    mafDataSet *data = inputVME->dataSetCollection()->itemAtCurrentTime();
    mafContainer<vtkAlgorithmOutput> *dataSet = mafContainerPointerTypeCast(vtkAlgorithmOutput, data->dataValue());

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

void mafVisualPipeSelection::updatePipe(double t) {
    mafVMEList *inputList = this->inputList();
    mafDataSet *data = inputList->at(0)->dataSetCollection()->itemAt(t);
    if(data == NULL) {
        return;
    }

    mafContainer<vtkAlgorithmOutput> *dataSet = mafContainerPointerTypeCast(vtkAlgorithmOutput, data->dataValue());
    m_OutlineCornerFilter->SetInputConnection(*dataSet);
    m_OutlineCornerFilter->Update();
}
