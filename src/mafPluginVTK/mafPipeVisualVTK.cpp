/*
 *  mafPipeVisualVTK.cpp
 *  mafPluginVTK
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisualVTK.h"
#include "mafVTKWidget.h"

#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkProp.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;
using namespace std;

mafPipeVisualVTK::mafPipeVisualVTK(const QString code_location) : mafPipeVisual(code_location), m_ScalarVisibility(false), m_ImmediateRendering(false), m_Renderer(NULL) {
}

mafPipeVisualVTK::~mafPipeVisualVTK() {
}

void mafPipeVisualVTK::updatedGraphicObject() {
    mafVTKWidget* widget = qobject_cast<mafVTKWidget*>(graphicObject());
    vtkRendererCollection *rc = widget->GetRenderWindow()->GetRenderers();
    m_Renderer = rc->GetFirstRenderer();
}


void mafPipeVisualVTK::setScalarVisibility(bool scalarVisibility) {
    m_ScalarVisibility = scalarVisibility;
}

void mafPipeVisualVTK::setImmediateRendering (bool immediateRendering) {
    m_ImmediateRendering = immediateRendering;
}

void mafPipeVisualVTK::updateVisibility(vtkProp *prop) {
    prop->SetVisibility(visibility());
    
    if (graphicObject() == NULL) {
        return;
    }
    if(visibility()) {
        m_Renderer->AddViewProp(prop);
    } else {
        m_Renderer->RemoveViewProp(prop);
    }
    m_Renderer->GetRenderWindow()->Render();
}

