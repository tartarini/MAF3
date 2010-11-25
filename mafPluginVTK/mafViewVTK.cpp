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

mafViewVTK::mafViewVTK(const mafString code_location) : mafView(code_location) , m_EventBridge(NULL) , m_Iren(NULL), m_Renderer(NULL){
}

mafViewVTK::~mafViewVTK() {
    mafDEL(m_EventBridge);
    if(m_Iren) m_Iren->Delete();
    if(m_Renderer) m_Renderer->Delete();
}

void mafViewVTK::create() {
    Superclass::create();

    m_RenWin = mafContainerPointerTypeCast(vtkRenderWindow, m_RenWindow);
    m_Renderer = vtkRenderer::New();
    m_Iren = vtkRenderWindowInteractor::New();
    m_EventBridge = mafNEW(mafPluginVTK::mafEventBridgeVTK);

    (*m_RenWin)->AddRenderer(m_Renderer);
    m_Iren->SetRenderWindow(*m_RenWin);
    m_RenWin->setDestructionFunction(&vtkRenderWindow::Delete);

    m_Renderer->SetBackground(0.1, 0.1, 0.1);
    (*m_RenWin)->SetSize(640, 480);
    (*m_RenWin)->SetPosition(400,0);

    m_EventBridge->setInteractor(m_Iren);
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

        if ((*actor)->GetVisibility() == 0) {
            m_Renderer->AddActor(*actor);
            // To be revised.
            //node->m_IsRendered = true;
        }
        (*actor)->SetVisibility(show);
        (*m_RenWin)->Render();
        //m_Iren->Start();
    }
}


