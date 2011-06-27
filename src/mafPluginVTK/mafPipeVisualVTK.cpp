/*
 *  mafPipeVisualVTK.cpp
 *  mafPluginVTK
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 30/12/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisualVTK.h"
#include "mafVTKWidget.h"
#include "mafSceneNodeVTK.h"

#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkProp3D.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;


mafPipeVisualVTK::mafPipeVisualVTK(const QString code_location) : mafPipeVisual(code_location), m_ScalarVisibility(false), m_ImmediateRendering(false), m_Renderer(NULL) {
}

mafPipeVisualVTK::~mafPipeVisualVTK() {
    m_GraphicObject = NULL;
    setVisibility(false);
}

void mafPipeVisualVTK::updatedGraphicObject() {
    mafVTKWidget *widget = qobject_cast<mafVTKWidget *>(graphicObject());
    vtkRendererCollection *rc = widget->GetRenderWindow()->GetRenderers();
    m_Renderer = rc->GetFirstRenderer();
}

void mafPipeVisualVTK::render() {
    mafVTKWidget *widget = qobject_cast<mafVTKWidget *>(graphicObject());
    if (widget != NULL) {
        widget->GetRenderWindow()->Render();
    }
}

void mafPipeVisualVTK::setScalarVisibility(bool scalarVisibility) {
    m_ScalarVisibility = scalarVisibility;
}

void mafPipeVisualVTK::setImmediateRendering (bool immediateRendering) {
    m_ImmediateRendering = immediateRendering;
}

void mafPipeVisualVTK::updateVisibility() {
    m_Prop3D->SetVisibility(visibility());
    
    if (graphicObject() == NULL) {
        return;
    }
    
    mafSceneNodeVTK *sn = (mafSceneNodeVTK *)sceneNode();
    if(sn == NULL) {
        return;
    }
    vtkProp *prop = (vtkProp *)sn->nodeAssembly();
    if(visibility()) {
        m_Renderer->AddViewProp(prop);
    } else {
        m_Renderer->RemoveViewProp(prop);
    }
    render();
    m_Renderer->ResetCamera();
}
