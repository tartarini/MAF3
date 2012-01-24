/*
 *  mafPipeVisualVTK.cpp
 *  mafPluginVTK
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 30/12/09.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisualVTK.h"
#include "mafVTKWidget.h"

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

using namespace mafPluginVTK;


mafPipeVisualVTK::mafPipeVisualVTK(const QString code_location) : mafResources::mafPipeVisual(code_location), m_ScalarVisibility(false), m_ImmediateRendering(false), m_Renderer(NULL) {
}

mafPipeVisualVTK::~mafPipeVisualVTK() {
    m_Prop3D->SetVisibility(false);
}

void mafPipeVisualVTK::updatedGraphicObject() {
    mafVTKWidget *widget = qobject_cast<mafVTKWidget *>(graphicObject());
    m_Renderer = widget->renderer();
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
}

void mafPipeVisualVTK::updatePipe(double t) {
    // Reset the modified state of the pipe. 
    // Now everything is up to date.
    setModified(false);
}
