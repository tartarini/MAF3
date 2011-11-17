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

using namespace mafPluginVTK;


mafToolVTK::mafToolVTK(const QString code_location) : mafResources::mafTool(code_location), m_RendererTool(NULL) {
}

mafToolVTK::~mafToolVTK() {
}

void mafToolVTK::updatedGraphicObject() {
    mafVTKWidget *widget = qobject_cast<mafVTKWidget *>(m_GraphicObject);
    if (widget) {
        m_RendererTool = widget->renderer("tool");
        Q_EMIT graphicObjectReady();
    }
}

void mafToolVTK::addProp(vtkProp3D *prop) {
    if (m_RendererTool) {
        m_RendererTool->AddViewProp(prop);
    }
}

void mafToolVTK::removeProp(vtkProp3D *prop) {
    if (m_RendererTool) {
        m_RendererTool->RemoveViewProp(prop);
    }
}

void mafToolVTK::addWidget(vtkInteractorObserver *w) {
}

void mafToolVTK::removeWidget(vtkInteractorObserver *w) {
}
