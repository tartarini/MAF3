/*
 *  mafViewVTK.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 30/03/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
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

mafViewVTK::mafViewVTK(const QString code_location) : mafView(code_location), m_Renderer(NULL), m_CameraParallel(false), m_CameraAxesDirection(mafCameraDirectionZNegative) {
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
        m_Renderer->SetBackground(0.1, 0.1, 0.1);
        ((mafVTKWidget*)m_RenderWidget)->showAxes(true);
        ((mafVTKWidget*)m_RenderWidget)->setParallelCameraMode(m_CameraParallel);
        setCameraAxes(m_CameraAxesDirection);
    
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

void mafViewVTK::setCameraAxes(int axes) {
    m_CameraAxesDirection = (mafCameraDirection)axes;
    if (m_RenderWidget == NULL) {
        return;
    }

    double position[3], focalPoint[3], viewUp[3];
    switch (m_CameraAxesDirection) {
            case mafCameraDirectionX:
            {
                position[0] = 0.;
                position[1] = 0.;
                position[2] = 0.;
                focalPoint[0] = 1.;
                focalPoint[1] = 0.; 
                focalPoint[2] = 0.;
                viewUp[0] = 0.;
                viewUp[1] = 1.;
                viewUp[2] = 0.;
            }
                break;
            case mafCameraDirectionXNegative:
            {
                position[0] = 0.;
                position[1] = 0.;
                position[2] = 0.;
                focalPoint[0] = -1.;
                focalPoint[1] = 0.; 
                focalPoint[2] = 0.;
                viewUp[0] = 0.;
                viewUp[1] = 1.;
                viewUp[2] = 0.;
            }
                break;
            case mafCameraDirectionY:
            {
                position[0] = 0.;
                position[1] = 0.;
                position[2] = 0.;
                focalPoint[0] = 0.;
                focalPoint[1] = 1.; 
                focalPoint[2] = 0.;
                viewUp[0] = 0.;
                viewUp[1] = 0.;
                viewUp[2] = 1.;
            }
                break;
            case mafCameraDirectionYNegative:
            {
                position[0] = 0.;
                position[1] = 0.;
                position[2] = 0.;
                focalPoint[0] = 0.;
                focalPoint[1] = -1.; 
                focalPoint[2] = 0.;
                viewUp[0] = 0.;
                viewUp[1] = 0.;
                viewUp[2] = 1.;
            }
                break;
            case mafCameraDirectionZ:
            {
                position[0] = 0.;
                position[1] = 0.;
                position[2] = 0.;
                focalPoint[0] = 0.;
                focalPoint[1] = 0.; 
                focalPoint[2] = 1.;
                viewUp[0] = 0.;
                viewUp[1] = 1.;
                viewUp[2] = 0.;
            }
                break;
            case mafCameraDirectionZNegative:
            {
                position[0] = 0.;
                position[1] = 0.;
                position[2] = 0.;
                focalPoint[0] = 0.;
                focalPoint[1] = 0.; 
                focalPoint[2] = -1.;
                viewUp[0] = 0.;
                viewUp[1] = 1.;
                viewUp[2] = 0.;
            }
                break;
            default:
            {
                position[0] = 0.;
                position[1] = 0.;
                position[2] = 0.;
                focalPoint[0] = 0.;
                focalPoint[1] = 0.; 
                focalPoint[2] = 1.;
                viewUp[0] = 0.;
                viewUp[1] = 1.;
                viewUp[2] = 0.;
            }
            break;
    }
    ((mafVTKWidget*)m_RenderWidget)->setCameraParameters(position, focalPoint,viewUp);
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
    ((mafVTKWidget*)m_RenderWidget)->update();
    if (bounds) {
        m_Renderer->ResetCamera(bounds);
    } else {
        m_Renderer->ResetCamera();
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
