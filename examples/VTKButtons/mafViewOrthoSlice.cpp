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

mafViewOrthoSlice::mafViewOrthoSlice(const QString code_location) : mafViewCompound(code_location) {
    m_PlaneTool[0] = NULL;
    m_PlaneTool[1] = NULL;
    m_PlaneTool[2] = NULL;

	setConfigurationFile("OrthoSlice.xml");
    m_GUI = new mafOrthoSlice();
    this->setUIRootWidget(m_GUI);
    connect(m_GUI, SIGNAL(positionUpdated(double *)), this, SLOT(sliceAtPosition(double *)));
}

mafViewOrthoSlice::~mafViewOrthoSlice() {
    for (int t = 0; t < 3; ++t) {
        mafDEL(m_PlaneTool[t]);
    }
}

void mafViewOrthoSlice::addPlaneToolsToHandler() {
    mafCore::mafPoint n[3];
    n[0] = mafCore::mafPoint(1., 0., 0.);
    n[1] = mafCore::mafPoint(0., 1., 0.);
    n[2] = mafCore::mafPoint(0., 0., 1.);
    for (int t = 0; t < 3; ++t) {
        m_PlaneTool[t] = mafNEW(mafPluginVTK::mafToolVTKPlane);
        m_PlaneTool[t]->setFollowSelectedObject(false);
        m_PlaneTool[t]->setFollowSelectedObjectVisibility(false);
        m_PlaneTool[t]->setNormal(n[t]);
    }
    Q_FOREACH(mafView *v, *viewList()) {
        v->toolHandler()->addTool(m_PlaneTool[0]);
        v->toolHandler()->addTool(m_PlaneTool[1]);
        v->toolHandler()->addTool(m_PlaneTool[2]);
    }
}

void mafViewOrthoSlice::sliceAtPosition(double *pos) {
    m_SlicePosition[0] = pos[0];
    m_SlicePosition[1] = pos[1];
    m_SlicePosition[2] = pos[2];
    // Trigger the pipe update
    setModified();
    updateView();
}

void mafViewOrthoSlice::showSceneNode(mafSceneNode *node, bool show) {
    Superclass::showSceneNode(node, show);
    if (show) {
        mafVME *vme = node->vme();
        double b[6];
        vme->bounds(b);
        m_GUI->setBounds(b);
        const double *p = m_GUI->position();
        m_SlicePosition[0] = p[0];
        m_SlicePosition[1] = p[1];
        m_SlicePosition[2] = p[2];
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
