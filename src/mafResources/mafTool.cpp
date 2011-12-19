/*
 *  mafTool.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 27/09/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTool.h"
#include "mafSceneNode.h"

using namespace mafResources;


mafTool::mafTool(const QString code_location) : mafPipeVisual(code_location), m_SceneNode(NULL) {
    connect(this, SIGNAL(graphicObjectReady()), this, SLOT(graphicObjectInitialized()));
}

mafTool::~mafTool() {
}

void mafTool::setSceneNode(mafSceneNode *node) {
    if (m_SceneNode != node) {
        m_SceneNode = node;
    }
}

void mafTool::updatePipe(double t) {

}

void mafTool::graphicObjectInitialized() {
}

void mafTool::resetTool() {
}
