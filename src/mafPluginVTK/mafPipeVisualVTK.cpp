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
#include "mafAxes.h"

#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkProp3D.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;


mafPipeVisualVTK::mafPipeVisualVTK(const QString code_location) : mafPipeVisual(code_location), m_ScalarVisibility(false), m_ImmediateRendering(false), m_Renderer(NULL), m_Axes(NULL) {
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
    PRINT_FUNCTION_NAME_INFORMATION
    m_Prop3D->SetVisibility(visibility());
    
    if(visibility()) {
        if (m_Axes == NULL) {
            m_Axes = new mafAxes(m_Renderer, input());
        }
        m_Axes->setVisibility(true);
    } else {
        if (m_Axes) {
            delete m_Axes;
            m_Axes = NULL;
        }
    }
    render();
}

void mafPipeVisualVTK::updatePipe(double t) {
    if (m_Axes) {
        m_Axes->updatePose();
    }
}
