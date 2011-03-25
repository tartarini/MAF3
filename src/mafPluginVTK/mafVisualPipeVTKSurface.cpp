/*
 *  mafVisualPipeVTKSurface.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVisualPipeVTKSurface.h"
#include <mafDataSet.h>
#include <mafDataSetCollection.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkActor.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;
using namespace std;

mafVisualPipeVTKSurface::mafVisualPipeVTKSurface(const QString code_location) : mafVisualPipe(code_location), m_Mapper(NULL), m_ScalarVisibility(0), m_ImmediateRendering(0) {
}

mafVisualPipeVTKSurface::~mafVisualPipeVTKSurface() {
    m_Mapper->Delete();
}

bool mafVisualPipeVTKSurface::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        mafCore::mafContainerInterface *data = vme->dataSetCollection()->itemAtCurrentTime()->dataValue();
        if(data != NULL) {
            QString dataType = data->externalDataType();
            if(dataType == "vtkAlgorithmOutput") {
                return true;
            }
        }
    }
    return false;
}

void mafVisualPipeVTKSurface::createPipe() {
    m_Mapper = vtkPolyDataMapper::New();
    m_Actor = vtkActor::New();
    m_Actor.setDestructionFunction(&vtkActor::Delete);
    m_Actor->SetMapper(m_Mapper);
    m_Output = &m_Actor;
}

void mafVisualPipeVTKSurface::updatePipe(double t) {
    Q_UNUSED(t);

    mafVME *inputVME = this->inputList()->at(0);
    mafDataSet *data = inputVME->dataSetCollection()->itemAtCurrentTime();
    mafContainer<vtkAlgorithmOutput> *dataSet = mafContainerPointerTypeCast(vtkAlgorithmOutput, data->dataValue());

    //Get data contained in the mafContainer
    m_Mapper->SetInputConnection(*dataSet);

    m_Mapper->SetScalarVisibility(m_ScalarVisibility);
    m_Mapper->SetImmediateModeRendering(m_ImmediateRendering);
}

void mafVisualPipeVTKSurface::setScalarVisibility(bool scalarVisibility) {
    if(m_ScalarVisibility != scalarVisibility) {
        m_ScalarVisibility = scalarVisibility;
    }
}

void mafVisualPipeVTKSurface::setImmediateRendering (bool immediateRendering) {
    if(m_ImmediateRendering != immediateRendering) {
        m_ImmediateRendering = immediateRendering;
    }
}

