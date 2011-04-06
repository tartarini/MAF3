/*
 *  mafSceneNode.cpp
 *  mafResources
 *
 *  Created by Daniele Giunchi on 24/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafSceneNode.h"
#include "mafPipeVisual.h"
#include "mafVME.h"

using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

mafSceneNode::mafSceneNode(const QString code_location) : mafObject(code_location), m_VisualPipe(NULL) {
}

mafSceneNode::mafSceneNode(mafVME *vme, mafPipeVisual *visual_pipe, const QString code_location) : mafObject(code_location), m_VME(vme), m_VisualPipe(visual_pipe), m_Visibility(false), m_CanVisualize(false) {
    connect(vme, SIGNAL(destroyed()), this, SIGNAL(destroyNode()));
    connect(vme, SIGNAL(detatched()), this, SIGNAL(destroyNode()));
    m_VME = vme;
    m_VisualPipe = visual_pipe;
    this->setProperty("iconFile",m_VME->property("iconFile"));
}

mafSceneNode::~mafSceneNode() {
    mafDEL(this->m_VisualPipe);
}

void mafSceneNode::setSelected(bool sel) {
    if(m_Selected != sel) {
        m_Selected = sel;
        if(m_Selected) {
            // notify the VME selection.
            m_VME->setSelected(true);
        }
    }
}

void mafSceneNode::visualPipeDestroyed() {
    this->m_VisualPipe = NULL;
}

void mafSceneNode::setVisualPipe(QString visualPipeType) {
    if(m_VisualPipe != NULL) {
        QString currentVisualPipeType = this->m_VisualPipe->metaObject()->className();
        if (visualPipeType.compare(currentVisualPipeType) == 0) {
            return;
        }
    }

    mafDEL(this->m_VisualPipe);
    this->m_VisualPipe = (mafPipeVisual *)mafNEWFromString(visualPipeType);
    connect(m_VisualPipe, SIGNAL(destroyed()), this, SLOT(visualPipeDestroyed()));
}

void mafSceneNode::setVisibility(bool visible) {
    m_Visibility = visible;
}

void mafSceneNode::setCanVisualize(bool canVisualize) {
    m_CanVisualize = canVisualize;
}

bool mafSceneNode::canVisualize() {
    m_CanVisualize = m_VME->property("canRead").toBool() ? true : false;
    m_CanVisualize = (m_VME->objectName() != "root");
    return m_CanVisualize;
}


