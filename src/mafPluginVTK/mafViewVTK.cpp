/*
 *  mafViewVTK.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 30/03/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#include "mafViewVTK.h"
#include "mafVTKWidget.h"
#include "mafSceneNodeVTK.h"
#include "mafPipeVisualVTKSelection.h"
#include "mafInteractorVTKCamera.h"

#include <mafPipeVisual.h>
#include <mafVME.h>
#include <mafHierarchy.h>
#include <mafVisitorFindSceneNodeByVMEHash.h>

// vtk
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAssembly.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;
using namespace mafEventBus;

mafViewVTK::mafViewVTK(const QString code_location) : mafView(code_location), m_Renderer(NULL) {
}

mafViewVTK::~mafViewVTK() {
    mafDEL(m_PipeVisualSelection);
    if(m_Renderer) {
        m_Renderer->Delete();
    }
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
    ((mafVTKWidget*)m_RenderWidget)->showAxes(true);
    
    //create the instance for selection pipe.
    m_PipeVisualSelection = mafNEW(mafPluginVTK::mafPipeVisualVTKSelection);
    m_PipeVisualSelection->setGraphicObject(m_RenderWidget);
    
    // push camera interactor
    mafInteractorVTKCamera *interactor = mafNEW(mafPluginVTK::mafInteractorVTKCamera);
    vtkRenderWindowInteractor *iren = ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->GetInteractor();
    interactor->setInteractorVTK(iren);
    pushInteractor(interactor);
    mafDEL(interactor);
}

mafSceneNode *mafViewVTK::createSceneNode(mafVME *vme) {
    mafSceneNode *sceneNode = new mafSceneNodeVTK(vme, m_RenderWidget, "", mafCodeLocation); 
    return sceneNode;
}


void mafViewVTK::removeSceneNode(mafResources::mafSceneNode *node) {
    if (node != NULL && node->visualPipe()) {
        mafSceneNodeVTK *n = qobject_cast<mafSceneNodeVTK *>(node);
        if(n) {
            m_Renderer->RemoveViewProp(n->nodeAssembly());
            mafSceneNodeVTK *nParent = qobject_cast<mafSceneNodeVTK *>(node->parentNode());
            if (nParent) {
                nParent->nodeAssembly()->RemovePart(n->nodeAssembly());
            }
            
        }
    }
    Superclass::removeSceneNode(node);
}

void mafViewVTK::showSceneNode(mafResources::mafSceneNode *node, bool show) {
    Superclass::showSceneNode(node, show);
    
    if(m_VisibleObjects == 1) {
        ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
    }

    ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->Render();
}

void mafViewVTK::updateView() {
  if (((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()) {
    ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->Render();
  }
}

void mafViewVTK::selectSceneNode(mafResources::mafSceneNode *node, bool select) {
    // remove from scene old selection
    mafProxy<vtkProp3D> *prop = mafProxyPointerTypeCast(vtkProp3D, m_PipeVisualSelection->output());
    mafSceneNodeVTK *selected = qobject_cast<mafSceneNodeVTK *>(m_SelectedNode);
    if(selected) {
        selected->nodeAssembly()->RemovePart(*prop);
        m_PipeVisualSelection->render();
    }
    //select new scenenode
    mafView::selectSceneNode(node,select);
    
    if(node && node->property("visibility").toBool()) {
        mafSceneNodeVTK *sn = qobject_cast<mafSceneNodeVTK *>(node);
        if(select) {
            sn->nodeAssembly()->AddPart(*prop);
        } 
        sn->update();
        m_PipeVisualSelection->render();
    } 
}
