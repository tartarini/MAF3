/*
 *  mafViewVTK.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 30/03/10.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */


#include "mafViewVTK.h"
#include "mafVTKWidget.h"
#include "mafSceneNodeVTK.h"
#include "mafInteractorVTKCamera.h"

#include "mafToolVTKSelection.h"
#include "mafToolVTKAxes.h"

#include <mafToolHandler.h>
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
}

bool mafViewVTK::initialize() {
    if (Superclass::initialize()) {
        // Create the instance of the VTK Widget
        m_RenderWidget = new mafVTKWidget();
        m_RenderWidget->setObjectName("VTKWidget");
        m_ToolHandler->setGraphicObject((mafVTKWidget*)m_RenderWidget);
        
        QVariant v;
        v.setValue<QObject*>(this);
        ((mafVTKWidget*)m_RenderWidget)->setViewObject(v);
        
        m_Renderer = ((mafVTKWidget*)m_RenderWidget)->renderer();
        m_Renderer->SetBackground(0.1, 0.1, 0.1);
        ((mafVTKWidget*)m_RenderWidget)->showAxes(true);
        ((mafVTKWidget*)m_RenderWidget)->setParallelCameraMode(m_CameraParallel);
        
        setCameraAxes(m_CameraAxesDirection);

        // Add the axes tool to the handler.
        mafToolVTKAxes *axesTool = mafNEW(mafPluginVTK::mafToolVTKAxes);
        m_ToolHandler->addTool(axesTool);
        mafDEL(axesTool);
    
        //create the instance for selection pipe.
        mafToolVTKSelection *toolSelection = mafNEW(mafPluginVTK::mafToolVTKSelection);
        m_ToolHandler->addTool(toolSelection);
        mafDEL(toolSelection);

        // push camera interactor
        mafInteractorVTKCamera *interactor = mafNEW(mafPluginVTK::mafInteractorVTKCamera);
        interactor->setGraphicObject(m_RenderWidget);
        pushInteractor(interactor);
        mafDEL(interactor);

        // Call always at the end of the initialization process to say the mafView to fill the SceneGraph
        // with the actual hierarchy present in the VME tree.
        setupSceneGraph();
        return true;
    }
    return false;
}

void mafViewVTK::showSceneNode(mafResources::mafSceneNode *node, bool show /* = true */) {
    Superclass::showSceneNode(node, show);

    if(show && m_VisibleObjects == 1) {
        resetVisualization();
    }
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
    updateView();
}
