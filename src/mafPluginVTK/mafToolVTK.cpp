/*
 *  mafToolVTK.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 27/09/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafToolVTK.h"
#include "mafSceneNodeVTK.h"
#include "mafVTKWidget.h"

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkAssembly.h>
#include <vtkInteractorObserver.h>

using namespace mafPluginVTK;


mafToolVTK::mafToolVTK(const QString code_location) : mafResources::mafTool(code_location), m_RendererTool(NULL) {
}

mafToolVTK::~mafToolVTK() {
    setSceneNode(NULL);
    m_PropList.clear();
    m_WidgetList.clear();
}

void mafToolVTK::updatedGraphicObject() {
    mafVTKWidget *widget = qobject_cast<mafVTKWidget *>(m_GraphicObject);
    if (widget) {
        if (m_RendererTool == NULL) {
            m_RendererTool = widget->renderer("tool");
            Q_EMIT graphicObjectReady();
        }
        m_RendererTool->GetRenderWindow()->Render();
    }
}

void mafToolVTK::addProp(vtkProp3D *prop) {
    if (m_RendererTool && prop && !m_PropList.contains(prop)) {
        m_RendererTool->AddViewProp(prop);
        m_PropList.append(prop);
    }
}

void mafToolVTK::removeProp(vtkProp3D *prop) {
    if (m_RendererTool && prop) {
        m_RendererTool->RemoveViewProp(prop);
        m_PropList.removeOne(prop);
    }
}

void mafToolVTK::addWidget(vtkInteractorObserver *w) {
    if (w && !m_WidgetList.contains(w)) {
        w->SetInteractor(m_RendererTool->GetRenderWindow()->GetInteractor());
        w->SetCurrentRenderer(m_RendererTool);
        m_WidgetList.append(w);
    }
}

void mafToolVTK::removeWidget(vtkInteractorObserver *w) {
    if (w) {
        m_WidgetList.removeOne(w);
    }
}

void mafToolVTK::setSceneNode(mafResources::mafSceneNode *node) {
    vtkAssembly *assembly = NULL;
    if (m_SceneNode) {
        // remove the axes from the old scene node...
        assembly = ((mafSceneNodeVTK *)m_SceneNode)->nodeAssembly();
        Q_FOREACH(vtkProp3D *prop, m_PropList) {
            assembly->RemovePart(prop);
        }
    }
    // initialize the member variable...
    Superclass::setSceneNode(node);

    // ... then add the axes actor to the new assembly...
    if (m_SceneNode) {
        assembly = ((mafSceneNodeVTK *)m_SceneNode)->nodeAssembly();
        Q_FOREACH(vtkProp3D *prop, m_PropList) {
            assembly->AddPart(prop);
        }
    }
}

void mafToolVTK::updateVisibility() {
    // Update the visibility of the actor associated with the tool.
    Q_FOREACH(vtkProp3D *prop, m_PropList) {
        prop->SetVisibility(visibility() ? 1 : 0);
    }
    Q_FOREACH(vtkInteractorObserver *widget, m_WidgetList) {
        widget->SetEnabled(visibility() ? 1 : 0);
    }
}
