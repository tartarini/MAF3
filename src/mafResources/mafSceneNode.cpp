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

mafSceneNode::mafSceneNode(const QString code_location) : mafObject(code_location), m_VisualPipe(NULL), m_VisualPipeType(""), m_VME(NULL), m_VisibilityPolicy(mafVisibilityPolicyDestroyOnHide) {
}

mafSceneNode::mafSceneNode(mafVME *vme, const QString visualPipeType, const QString code_location): mafObject(code_location), m_VME(vme), m_VisualPipe(NULL),m_VisualPipeType(visualPipeType), m_Visibility(false), m_VisualizationStatus(mafVisualizationStatusVisible), m_VisibilityPolicy(mafVisibilityPolicyDestroyOnHide) {

    REQUIRE(!visualPipeType.isEmpty());
    connect(vme, SIGNAL(detatched()), this, SIGNAL(destroyNode()));
    m_VME = vme;
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
    if(m_VisualPipe != NULL && visualPipeType.compare(m_VisualPipeType) == 0) {
        return;
    }
    
    if(visualPipeType.isEmpty()) {
        qCritical(mafTr("Pipe name is empty string, impossible to instantiate Visual pipe!").toAscii().constData());
        return;
    }
    
    m_VisualPipeType = visualPipeType;
    mafDEL(this->m_VisualPipe);
    this->m_VisualPipe = (mafPipeVisual *)mafNEWFromString(m_VisualPipeType);
    
    if(m_VisualPipe == NULL) {
        qWarning() << mafTr("No visual pipe type '") << m_VisualPipeType << mafTr("'' registered!!");
        return;
    }
    
    connect(m_VisualPipe, SIGNAL(destroyed()), this, SLOT(visualPipeDestroyed()));
    m_VisualPipe->setInput(m_VME);

    //if (m_VisualPipe->output() == NULL) {
    m_VisualPipe->createPipe();
    //}
    m_VisualPipe->updatePipe();
}

void mafSceneNode::setVMEName(QString name) {
  if(m_VME) {
    m_VME->setProperty("objectName", name);
  }
}

QString mafSceneNode::VMEName() {
  if(m_VME) {
    return m_VME->property("objectName").toString();
  }
}

void mafSceneNode::setVisibility(bool visible) {
    m_Visibility = visible;
    if(m_VisualPipe == NULL) {
        return;
    }
    m_VisualPipe->setVisibility(visible);
    
    if(!visible) {
        // TODO NEED TO IMPLEMENT A STRATEGY
        switch(m_VisibilityPolicy) {
            case mafVisibilityPolicyKeepAlive:
            {
                //will forward to visual pipe
            }
                break;
            case mafVisibilityPolicyDestroyOnHide:
            {
                mafDEL(m_VisualPipe);    
            }
            break;
            case mafVisibilityPolicySmartMemory:
                break;
        }
    }
}

void mafSceneNode::setVisualizationStatus(bool visualizationStatus) {
    m_VisualizationStatus = (mafVisualizationStatus) visualizationStatus;
}

unsigned int mafSceneNode::visualizationStatus() {
    return m_VisualizationStatus;
}

void mafSceneNode::setVisibilityPolicy(unsigned int visibilityPolicy) {
    m_VisibilityPolicy = (mafVisibilityPolicy) visibilityPolicy;
}

QString mafSceneNode::dataHash() const {
    return m_VME->objectHash();
}



