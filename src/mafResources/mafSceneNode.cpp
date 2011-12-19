/*
 *  mafSceneNode.cpp
 *  mafResources
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 24/03/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafSceneNode.h"
#include "mafPipeVisual.h"
#include "mafVME.h"

using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

mafSceneNode::mafSceneNode(const QString code_location) : mafObject(code_location), 
                                                          m_GraphicObject(NULL), 
                                                          m_VME(NULL), 
                                                          m_VisualPipeType(""), 
                                                          m_VisualPipe(NULL), 
                                                          m_Visibility(false), 
                                                          m_VisualizationStatus(mafVisualizationStatusVisible), 
                                                          m_VisibilityPolicy(mafVisibilityPolicyDestroyOnHide), 
                                                          m_ParentNode(NULL) {
}

mafSceneNode::mafSceneNode(mafVME *vme, QObject *graphicObject, const QString visualPipeType, const QString code_location) : 
                                                                mafObject(code_location), 
                                                                m_GraphicObject(graphicObject), 
                                                                m_VME(vme), 
                                                                m_VisualPipeType(visualPipeType), 
                                                                m_VisualPipe(NULL),
                                                                m_Visibility(false), 
                                                                m_VisualizationStatus(mafVisualizationStatusVisible), 
                                                                m_VisibilityPolicy(mafVisibilityPolicyDestroyOnHide), 
                                                                m_ParentNode(NULL) {
}

bool mafSceneNode::initialize() {
    if (Superclass::initialize()) {
        REQUIRE(m_VME);
        connect(m_VME, SIGNAL(detatched()), this, SIGNAL(destroyNode()));
        this->setProperty("iconFile",m_VME->property("iconFile"));
        this->setObjectName(m_VME->objectName());
        return true;
    }
    return false;
}

mafSceneNode::~mafSceneNode() {
    mafDEL(this->m_VisualPipe);
}

void mafSceneNode::setParentNode(const mafSceneNode *parent) {
    m_ParentNode = const_cast<mafSceneNode *>(parent);
}

void mafSceneNode::setVisualPipe(QString visualPipeType) {
    if(m_VisualPipe != NULL && visualPipeType.compare(m_VisualPipeType) == 0) {
        return;
    }
    
    if(visualPipeType.isEmpty()) {
        QByteArray ba = mafTr("Pipe type not valid, impossible create Visual pipe!").toAscii();
        qCritical(ba.constData());
        return;
    }
    
    // Update the visual pipe type with the new value
    m_VisualPipeType = visualPipeType;
    // and destroy the previous instance.
    mafDEL(this->m_VisualPipe);
}

bool mafSceneNode::createVisualPipe() {
    mafDEL(this->m_VisualPipe);
    this->m_VisualPipe = (mafPipeVisual *)mafNEWFromString(m_VisualPipeType);
    if(m_VisualPipe == NULL) {
        qWarning() << mafTr("No visual pipe type '") << m_VisualPipeType << mafTr("'' registered!!");
        return false;
    }

    this->m_VisualPipe->setGraphicObject(m_GraphicObject);
    
    m_VisualPipe->setInput(m_VME);
//    m_VisualPipe->updatePipe();
    return true;
}

void mafSceneNode::setVMEName(QString name) {
    if(m_VME) {
        m_VME->setProperty("objectName", name);
        setObjectName(name);
    }
}

QString mafSceneNode::VMEName() {
    if(m_VME) {
        return m_VME->property("objectName").toString();
    }
    return "";
}

void mafSceneNode::setVisibility(bool visible) {
    if (visible != m_Visibility) {
        if (m_VisualPipeType.isEmpty()) {
            return;
        }

        if(visible && m_VisualPipe == NULL) {
            if (!createVisualPipe()) {
                return;
            }
        }

        m_Visibility = visible;
        m_VisualPipe->setVisibility(visible);

        if(!visible) {
            // TODO NEED TO IMPLEMENT A STRATEGY
            switch(m_VisibilityPolicy) {
            case mafVisibilityPolicyKeepAlive: {
                    //will forward to visual pipe
                }
                break;
            case mafVisibilityPolicyDestroyOnHide: {
                    mafDEL(m_VisualPipe);    
                }
                break;
            case mafVisibilityPolicySmartMemory:
                break;
            }
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

QObject *mafSceneNode::dataObject() {
    QObject *obj = m_VME;
    return obj;
}
