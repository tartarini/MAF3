/*
 *  mafPipeVisualVTKSurface.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisualVTKSurface.h"
#include "mafVTKWidget.h"

#include <mafDataSet.h>
#include <mafDataSetCollection.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkActor.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;
using namespace std;

mafPipeVisualVTKSurface::mafPipeVisualVTKSurface(const QString code_location) : mafPipeVisual(code_location), m_Mapper(NULL), m_ScalarVisibility(false), m_ImmediateRendering(false), m_Renderer(NULL) {
    m_Mapper = vtkPolyDataMapper::New();
    m_Actor = vtkActor::New();
    m_Actor.setDestructionFunction(&vtkActor::Delete);
    m_Actor->SetMapper(m_Mapper);
    m_Output = &m_Actor;
}

mafPipeVisualVTKSurface::~mafPipeVisualVTKSurface() {
    setVisibility(false);
    m_Mapper->Delete();
}

bool mafPipeVisualVTKSurface::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        QString dataType = vme->dataSetCollection()->itemAtCurrentTime()->externalDataType();
        if(dataType.startsWith("vtkPolyData", Qt::CaseSensitive)) {
            return true;
        }
    }
    return false;
}

void mafPipeVisualVTKSurface::updatedGraphicObject() {
    mafVTKWidget* widget = qobject_cast<mafVTKWidget*>(graphicObject());
    vtkRendererCollection *rc = widget->GetRenderWindow()->GetRenderers();
    m_Renderer = rc->GetFirstRenderer();
}

void mafPipeVisualVTKSurface::updatePipe(double t) {
    mafVME *inputVME = this->inputList()->at(0);
    mafDataSet *data = inputVME->dataSetCollection()->itemAt(t);
    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, data->dataValue());
    
    //Get data contained in the mafProxy
    m_Mapper->SetInputConnection(*dataSet);
    m_Mapper->SetScalarVisibility(m_ScalarVisibility);
    m_Mapper->SetImmediateModeRendering(m_ImmediateRendering);
}

/// Set the visibility of its rendering scene.
void mafPipeVisualVTKSurface::setVisibility(bool visible) {
    Superclass::setVisibility(visible);
    m_Actor->SetVisibility(visible);
    
    if (graphicObject() == NULL) {
        return;
    }
    if(visible) {
        m_Renderer->AddActor(m_Actor);
    } else {
        m_Renderer->RemoveActor(m_Actor);
    }
    m_Renderer->GetRenderWindow()->Render();
}


void mafPipeVisualVTKSurface::setScalarVisibility(bool scalarVisibility) {
    m_ScalarVisibility = scalarVisibility;
}

void mafPipeVisualVTKSurface::setImmediateRendering (bool immediateRendering) {
    m_ImmediateRendering = immediateRendering;
}
