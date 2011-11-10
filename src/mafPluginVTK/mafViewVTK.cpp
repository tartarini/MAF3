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

// VTK
#include <vtkMAFInteractorStyleTrackballCamera.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAssembly.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;
using namespace mafEventBus;

mafViewVTK::mafViewVTK(const QString code_location) : mafView(code_location), m_Renderer(NULL) {
    m_SceneNodeType = "mafPluginVTK::mafSceneNodeVTK";
}

mafViewVTK::~mafViewVTK() {
    mafDEL(m_PipeVisualSelection);
}

bool mafViewVTK::initialize() {
    if (Superclass::initialize()) {
        // Create the instance of the VTK Widget
        m_RenderWidget = new mafVTKWidget();
        m_RenderWidget->setObjectName("VTKWidget");
        QVariant v;
        v.setValue<QObject*>(this);
        ((mafVTKWidget*)m_RenderWidget)->setViewObject(v);
        m_Renderer = ((mafVTKWidget*)m_RenderWidget)->renderer();
        ((mafVTKWidget*)m_RenderWidget)->showAxes(true);
    
        //create the instance for selection pipe.
        m_PipeVisualSelection = mafNEW(mafPluginVTK::mafPipeVisualVTKSelection);
        m_PipeVisualSelection->setGraphicObject(m_RenderWidget);
    
        // push camera interactor
        mafInteractorVTKCamera *interactor = mafNEW(mafPluginVTK::mafInteractorVTKCamera);
        vtkRenderWindowInteractor *iren = ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->GetInteractor();
        vtkMAFInteractorStyleTrackballCamera *interactorStyle = vtkMAFInteractorStyleTrackballCamera::New();
        interactorStyle->UseDefaultRendererOn();
        interactorStyle->SetDefaultRenderer(((mafVTKWidget*)m_RenderWidget)->renderer());
        iren->SetInteractorStyle(interactorStyle);
        interactorStyle->Delete();
        interactor->setInteractorVTK(iren);
        pushInteractor(interactor);
        mafDEL(interactor);

        // Call always at the end of the initialization process to say the mafView to fill the SceneGraph
        // with the actual hierarchy present in the VME tree.
        setupSceneGraph();
        return true;
    }
    return false;
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

void mafViewVTK::updateView() {
    if(m_VisibleObjects == 1) {
        resetVisualization();
    }

    if (((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()) {
        ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->Render();
    }
}

void mafViewVTK::resetVisualization(double *bounds) {
    if (bounds) {
        m_Renderer->ResetCamera(bounds);
    } else {
        double *b = ((mafVTKWidget*)m_RenderWidget)->visibleBounds();
        if (b) {
            m_Renderer->ResetCameraClippingRange(b);
        }
    }
}

void mafViewVTK::selectSceneNode(mafResources::mafSceneNode *node, bool select) {
    if (m_SelectedNode && m_SelectedNode->isEqual(node) && 
        m_PipeVisualSelection && m_PipeVisualSelection->input() &&
        m_PipeVisualSelection->input()->isEqual(m_SelectedNode->vme())) {
        return;
    }
    // remove from scene old selection
    mafProxy<vtkProp3D> *prop = mafProxyPointerTypeCast(vtkProp3D, m_PipeVisualSelection->output());
    mafSceneNodeVTK *selected = qobject_cast<mafSceneNodeVTK *>(m_SelectedNode);
    if(selected) {
        selected->nodeAssembly()->RemovePart(*prop);
    }
    //select new SceneNode
    mafView::selectSceneNode(node,select);
    
    if(node && node->property("visibility").toBool()) {
        mafSceneNodeVTK *sn = qobject_cast<mafSceneNodeVTK *>(node);
        if(select) {
            sn->nodeAssembly()->AddPart(*prop);
        } 
        sn->update();
    }
    updateView();
}
