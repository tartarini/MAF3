/*
 *  mafViewVTK.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 30/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#include "mafViewVTK.h"
#include "mafVTKWidget.h"
#include <mafPipeVisual.h>
#include <mafVME.h>
#include <mafHierarchy.h>
#include <mafSceneNode.h>
#include <mafVisitorFindSceneNodeByVMEHash.h>

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafViewVTK::mafViewVTK(const QString code_location) : mafView(code_location), m_Renderer(NULL) {
}

mafViewVTK::~mafViewVTK() {
    if(m_Renderer) {
        m_Renderer->Delete();
    }
    //mafDEL(m_Widget);
}

void mafViewVTK::create() {
    Superclass::create();
       
    // Create the instance of the VTK Widget
    m_RenderWidget = new mafVTKWidget();
    m_RenderWidget->setObjectName("VTKWidget");
    // Create the renderer
    m_Renderer = vtkRenderer::New();
    // and assign it to the widget.
    ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->AddRenderer(m_Renderer);
}

void mafViewVTK::removeSceneNode(mafResources::mafSceneNode *node) {
    if (node != NULL && node->visualPipe()) {
        mafContainer<vtkActor> *actor = mafContainerPointerTypeCast(vtkActor, node->visualPipe()->output());
        if ((*actor)->GetVisibility() != 0) {
            m_Renderer->RemoveActor(*actor);
        }
    }
    Superclass::removeSceneNode(node);
}

void mafViewVTK::showSceneNode(mafResources::mafSceneNode *node, bool show, const QString visualPipeType) {
    Superclass::showSceneNode(node, show, visualPipeType);

    ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->Render();
}

void mafViewVTK::updateView() {
  if (((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()) {
    ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->Render();
  }
}
