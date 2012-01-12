/*
 *  mafToolVTKAxes.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 27/09/11.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafToolVTKAxes.h"
#include "mafSceneNodeVTK.h"

#include <vtkAxesActor.h>

using namespace mafPluginVTK;


mafToolVTKAxes::mafToolVTKAxes(const QString code_location) : mafToolVTK(code_location) {
    m_AxesActor = vtkAxesActor::New();
    m_AxesActor->AxisLabelsOff();
    m_AxesActor->SetPickable(0);
    m_AxesActor->SetVisibility(0);
}

mafToolVTKAxes::~mafToolVTKAxes() {
    m_AxesActor->Delete();
}

void mafToolVTKAxes::resetTool() {
    removeProp(m_AxesActor);
}

void mafToolVTKAxes::graphicObjectInitialized() {
    // Graphic widget (render window, interactor...) has been created and initialized.
    // now can add the tool's actor.
    addProp(m_AxesActor);
}
