/*
 *  mafToolVTKAxes.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 27/09/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafToolVTKAxes.h"
#include "mafSceneNodeVTK.h"

#include <vtkAssembly.h>
#include <vtkAxesActor.h>

using namespace mafPluginVTK;


mafToolVTKAxes::mafToolVTKAxes(const QString code_location) : mafToolVTK(code_location) {
    m_AxesActor = vtkAxesActor::New();
    m_AxesActor->AxisLabelsOff();
    m_AxesActor->SetPickable(0);
    m_AxesActor->SetVisibility(0);
}

mafToolVTKAxes::~mafToolVTKAxes() {
    if (m_SceneNode) {
        vtkAssembly *assembly = ((mafSceneNodeVTK *)m_SceneNode)->nodeAssembly();
        assembly->RemovePart(m_AxesActor);
    }
    m_AxesActor->Delete();
}


void mafToolVTKAxes::resetTool() {
    removeProp(m_AxesActor);
}

void mafToolVTKAxes::setSceneNode(mafResources::mafSceneNode *node) {
    vtkAssembly *assembly = NULL;
    if (m_SceneNode) {
        // remove the axes from the old scene node...
        assembly = ((mafSceneNodeVTK *)m_SceneNode)->nodeAssembly();
        assembly->RemovePart(m_AxesActor);
    }
    // initialize the member variable...
    Superclass::setSceneNode(node);

    // ... then add the axes actor to the new assembly...
    if (m_SceneNode) {
        assembly = ((mafSceneNodeVTK *)m_SceneNode)->nodeAssembly();
        assembly->AddPart(m_AxesActor);
    }
}

void mafToolVTKAxes::graphicObjectInitialized() {
    addProp(m_AxesActor);
}

void mafToolVTKAxes::updateVisibility() {
    m_AxesActor->SetVisibility(visibility() ? 1 : 0);
}
