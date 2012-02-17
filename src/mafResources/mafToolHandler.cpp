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
#include "mafVME.h"

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
        // Tool already added. Do nothing.
        return;
    }
    // Retain the reference to the added tool, 
    // so who add it can release the reference count.
    tool->retain();
    if (m_GraphicObject) {
        // If graphic object is already in place, assign it to the tool
        tool->setGraphicObject(m_GraphicObject);
    }
    // Add the tool to the list.
    m_ToolList.append(tool);
}

void mafToolHandler::removeTool(mafTool *tool) {
    if (m_ToolList.contains(tool)) {
        // Remove the given tool from the list.
        m_ToolList.removeOne(tool);
        // Release its reference count.
        tool->release();
    }
}

void mafToolHandler::removeAllTools() {
    // Release the reference count for all tools.
    Q_FOREACH(mafTool *tool, m_ToolList) {
        tool->release();
    }
    // ... then clear the list.
    m_ToolList.clear();
}

void mafToolHandler::setActiveSceneNode(mafSceneNode *node) {
    if (node == m_SceneNode) {
        return;
    }
    // Keep track of the active scene node.
    m_SceneNode = node;
    Q_FOREACH(mafTool *tool, m_ToolList) {
        // Check the follow flag for each tool
        if (tool->followSelectedObject()) {
            // If tool has to be attached to the selected node, pass it to the tool.
            tool->setSceneNode(m_SceneNode);
            if (m_SceneNode) {
                tool->setInput(m_SceneNode->vme());
            }
        }
        if (tool->followSelectedObjectVisibility()) {
            bool v = m_SceneNode && m_SceneNode->property("visibility").toBool();
            // ... and update its visibility according to the node visibility.
            if (tool->followSelectedObject()) {
                //exist 1 instance of tool for ALL VMEs
                tool->setVisibility(v);
            } else {
                //exist 1 instance of tool for EACH VME
                if (m_SceneNode && tool->input()->isEqual(m_SceneNode->vme())) {
                    // ... and update its visibility according to the node visibility.
                    tool->setVisibility(v);
                }
            }
        }
        tool->updatedGraphicObject();
        tool->updatePipe();
    }
}

void mafToolHandler::setVisibility(bool visible) {
    // Update the visibility flag for all the tools.
    Q_FOREACH(mafTool *tool, m_ToolList) {
        if (tool->followSelectedObjectVisibility() && m_SceneNode && tool->input()->isEqual(m_SceneNode->vme())) {
            // ... and update its visibility according to the node visibility.
            tool->setVisibility(visible);
        }
    }
}
