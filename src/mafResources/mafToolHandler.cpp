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

using namespace mafResources;

mafToolHandler::mafToolHandler(const QString code_location) : mafCore::mafObjectBase(code_location) {
}

mafToolHandler::~mafToolHandler() {
    removeAllTools();
}

void mafToolHandler::addTool(mafTool *tool, bool follow_selection) {
    if (m_ToolList.contains(tool)) {
        return;
    }
    tool->retain();
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
