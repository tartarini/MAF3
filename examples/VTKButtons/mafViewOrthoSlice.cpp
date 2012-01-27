/*
 *  mafViewOrthoSlice.cpp
 *  VTKButtons
 *
 *  Created by Paolo Quadrani on 27/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafViewOrthoSlice.h"
#include "mafOrthoSlice.h"
#include <mafVME.h>
#include <mafPipeVisual.h>
#include <mafToolHandler.h>

using namespace mafResources;
using namespace mafPluginVTK;
using namespace mafCore;

mafViewOrthoSlice::mafViewOrthoSlice(const QString code_location) : mafViewCompound(code_location) {
    setConfigurationFile("OrthoSlice.xml");
    m_GUI = new mafOrthoSlice();
    this->setUIRootWidget(m_GUI);
    connect(m_GUI, SIGNAL(positionUpdated(double *)), this, SLOT(sliceAtPosition(double *)));
}

mafViewOrthoSlice::~mafViewOrthoSlice() {
    Q_FOREACH(mafToolVTKPlane *plane, m_PlaneTool) {
        mafDEL(plane);
    }
}

void mafViewOrthoSlice::addPlaneToolsToHandler() {
    mafCore::mafPoint n[3];
    n[0] = mafCore::mafPoint(1., 0., 0.);
    n[1] = mafCore::mafPoint(0., 1., 0.);
    n[2] = mafCore::mafPoint(0., 0., 1.);
    for (int t = 0; t < 3; ++t) {
        mafPluginVTK::mafToolVTKPlane *plane = mafNEW(mafPluginVTK::mafToolVTKPlane);
        plane->setFollowSelectedObject(false);
        plane->setFollowSelectedObjectVisibility(false);
        plane->setNormal(n[t]);
        m_PlaneTool.push_back(plane);
    }
    Q_FOREACH(mafView *v, *viewList()) {
        v->toolHandler()->addTool(m_PlaneTool.at(0));
        v->toolHandler()->addTool(m_PlaneTool.at(1));
        v->toolHandler()->addTool(m_PlaneTool.at(2));
    }
}

void mafViewOrthoSlice::sliceAtPosition(double *pos) {
    m_SlicePosition[0] = pos[0];
    m_SlicePosition[1] = pos[1];
    m_SlicePosition[2] = pos[2];
    // Trigger the pipe update

    Q_FOREACH(mafToolVTKPlane *plane, m_PlaneTool) {
        mafPoint o(m_SlicePosition);
        plane->setOrigin(o);
    }
    setModified();
    updateView();
}

void mafViewOrthoSlice::showSceneNode(mafSceneNode *node, bool show) {
    Superclass::showSceneNode(node, show);
    if (show) {
        mafVME *vme = node->vme();
        if (m_VisibleObjects == 1) {
            double b[6];
            vme->bounds(b);
            m_GUI->setBounds(b);
            const double *p = m_GUI->position();
            m_SlicePosition[0] = p[0];
            m_SlicePosition[1] = p[1];
            m_SlicePosition[2] = p[2];
        }
        mafView *subView;
        QList<mafView *> *vList = viewList();
        Q_FOREACH(subView, *vList) {
            mafSceneNode *sub_node = subView->sceneNodeFromVme((mafCore::mafObjectBase *)vme);
            mafPipeVisual *vp = sub_node->visualPipe();
            if (vp) {
                vp->setDelegateObject(this);
                bool res = connect(this, SIGNAL(modifiedObject()), vp, SLOT(updatePipe()));
                vp->updatePipe();
            }
        }
    }
    if (m_PlaneTool[0] == NULL) {
        addPlaneToolsToHandler();
    }
    Q_FOREACH(mafToolVTKPlane *plane, m_PlaneTool) {
        plane->setVisibility(m_VisibleObjects != 0);
    }
}

//////////////////////////////////////////////////////////////////////////
// Delegate methods
//////////////////////////////////////////////////////////////////////////
QString mafViewOrthoSlice::originX() {
    return QString::number(m_SlicePosition[0]);
}

QString mafViewOrthoSlice::originY() {
    return QString::number(m_SlicePosition[1]);
}

QString mafViewOrthoSlice::originZ() {
    return QString::number(m_SlicePosition[2]);
}

bool mafViewOrthoSlice::shouldExecuteLocalCode() {
    return false;
}
