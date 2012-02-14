/*
 *  mafViewOrthoSlice.cpp
 *  VTKButtons
 *
 *  Created by Paolo Quadrani on 27/10/11.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafViewOrthoSlice.h"
#include "mafOrthoSlice.h"
#include <mafVME.h>
#include <mafPipeVisual.h>
#include <mafToolHandler.h>
#include <mafBounds.h>
#include <mafPoint.h>

using namespace mafResources;
using namespace mafPluginVTK;
using namespace mafCore;

mafViewOrthoSlice::mafViewOrthoSlice(const QString code_location) : mafViewCompound(code_location), m_OrthoPlaneTool(NULL) {
    setConfigurationFile("OrthoSlice.xml");
    m_GUI = new mafOrthoSlice();
    this->setUIRootWidget(m_GUI);
    connect(m_GUI, SIGNAL(positionUpdated(double *)), this, SLOT(sliceAtPosition(double *)));
}

mafViewOrthoSlice::~mafViewOrthoSlice() {
    mafDEL(m_OrthoPlaneTool);
}

void mafViewOrthoSlice::addPlaneToolsToHandler() {
    mafCore::mafBounds *b; // Calculate the right bounds using the mafVisitorBounds
    mafCore::mafPoint *o = new mafCore::mafPoint(m_SlicePosition);
    Q_FOREACH(mafView *v, *viewList()) {
        m_OrthoPlaneTool = mafNEW(mafToolVTKOrthoPlane);
        m_OrthoPlaneTool->setFollowSelectedObject(false);
        m_OrthoPlaneTool->setFollowSelectedObjectVisibility(false);
        m_OrthoPlaneTool->setOrigin(o);
        m_OrthoPlaneTool->setVOI(b);
        mafToolHandler *handler = v->toolHandler();
        handler->addTool(m_OrthoPlaneTool);
    }
    mafDEL(b);
    mafDEL(o);
}

void mafViewOrthoSlice::sliceAtPosition(double *pos) {
    m_SlicePosition[0] = pos[0];
    m_SlicePosition[1] = pos[1];
    m_SlicePosition[2] = pos[2];
    // Trigger the pipe update

    mafPoint *o = new mafPoint(m_SlicePosition);
    m_OrthoPlaneTool->setOrigin(o);
    mafDEL(o);
    setModified();
    updateView();
}

void mafViewOrthoSlice::showSceneNode(mafSceneNode *node, bool show) {
    Superclass::showSceneNode(node, show);
    double b[6];
    if (show) {
        mafVME *vme = node->vme();
        vme->bounds(b);
        if (m_VisibleObjects == 1) {
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
    if (m_OrthoPlaneTool == NULL) {
        addPlaneToolsToHandler();
    }
    mafBounds *bounds = new mafBounds(b);
    m_OrthoPlaneTool->setSceneNode(node);
    m_OrthoPlaneTool->setVOI(bounds);
    m_OrthoPlaneTool->setVisibility(m_VisibleObjects != 0);
    mafDEL(bounds);
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
