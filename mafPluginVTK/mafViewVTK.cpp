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
#include <mafVisualPipe.h>
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

mafViewVTK::mafViewVTK(const mafString code_location) : mafView(code_location), m_Renderer(NULL) {
}

mafViewVTK::~mafViewVTK() {
    if(m_Renderer) {
        m_Renderer->Delete();
    }
}

void mafViewVTK::create() {
    Superclass::create();

    // Craete the instance of the VTK Widget
    m_Widget = new mafVTKWidget();
    m_Widget->setObjectName("VTKWidget");
    // Initialize the conteiner interface pointer of the base class
    m_RenderWidget = &m_Widget;

    // Create the renderer
    m_Renderer = vtkRenderer::New();
    // and assign it to the widget.
    m_Widget->GetRenderWindow()->AddRenderer(m_Renderer);
}

void mafViewVTK::removeVME(mafVME *vme) {
    mafVisitorFindSceneNodeByVMEHash *v = new mafVisitorFindSceneNodeByVMEHash(vme->objectHash(), mafCodeLocation);
    mafObjectRegistry::instance()->findObjectsThreaded(v);
    mafSceneNode *node = v->sceneNode();
    if (node != NULL) {
        mafContainer<vtkActor> *actor = mafContainerPointerTypeCast(vtkActor, node->visualPipe()->output());
        if ((*actor)->GetVisibility() != 0) {
            m_Renderer->RemoveActor(*actor);
        }
    }
    mafDEL(v);
    Superclass::removeVME(vme);
}

void mafViewVTK::showVME(mafVME *vme, bool show, const mafString visualPipeType) {
    Superclass::showVME(vme, show, visualPipeType);

    mafVisitorFindSceneNodeByVMEHash *v = new mafVisitorFindSceneNodeByVMEHash(vme->objectHash(), mafCodeLocation);
    mafObjectRegistry::instance()->findObjectsThreaded(v);
    mafSceneNode *node = v->sceneNode();
    mafDEL(v);

    if (node != NULL) {
        mafContainer<vtkActor> *actor = mafContainerPointerTypeCast(vtkActor, node->visualPipe()->output());
        m_Renderer->AddActor(*actor);
        if ((*actor)->GetVisibility() == 0) {
            // To be revised.
            //node->m_IsRendered = true;
        }
        (*actor)->SetVisibility(show);
        m_Widget->GetRenderWindow()->Render();
        //m_Iren->Start();
    }
}


