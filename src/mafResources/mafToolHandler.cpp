/*
 *  mafToolHandler.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 27/09/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafToolHandler.h"
#include "mafTool.h"
#include "mafSceneNode.h"

#include <mafEventBusManager.h>


using namespace mafResources;
using namespace mafEventBus;

mafToolHandler::mafToolHandler(const QString code_location) : mafCore::mafObjectBase(code_location), m_SceneNode(NULL), m_GraphicObject(NULL) {
}

mafToolHandler::~mafToolHandler() {
    removeAllTools();
}

void mafToolHandler::addTool(mafTool *tool) {
    if (m_ToolList.contains(tool)) {
        return;
    }
    tool->retain();
    if (m_GraphicObject) {
        tool->setGraphicObject(m_GraphicObject);
    }
    m_ToolList.append(tool);
}

void mafToolHandler::removeTool(mafTool *tool) {
    if (m_ToolList.contains(tool)) {
        m_ToolList.removeOne(tool);
        tool->release();
    }
}

void mafToolHandler::removeAllTools() {
    Q_FOREACH(mafTool *tool, m_ToolList) {
        tool->release();
    }
    m_ToolList.clear();
}

void mafToolHandler::setActiveSceneNode(mafSceneNode *node) {
    if (node == m_SceneNode) {
        return;
    }
    m_SceneNode = node;
    Q_FOREACH(mafTool *tool, m_ToolList) {
        if (tool->followSelectedObject()) {
            tool->setSceneNode(m_SceneNode);
            bool v = m_SceneNode && m_SceneNode->property("visibility").toBool();
            tool->setVisibility(v);
        }
    }
}

void mafToolHandler::setVisibility(bool visible) {
    Q_FOREACH(mafTool *tool, m_ToolList) {
        tool->setVisibility(visible);
    }
}
