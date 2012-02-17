/*
 *  mafViewVTKButtons.cpp
 *  VTKButtons
 *
 *  Created by Roberto Mucci on 17/01/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */


#include "mafViewVTKButtons.h"
#include "mafToolVTKButtons.h"
#include <mafSceneNode.h>
#include <mafToolHandler.h>

using namespace mafPluginVTK;

mafViewVTKButtons::mafViewVTKButtons(const QString code_location) : mafViewVTK(code_location) {
}

mafViewVTKButtons::~mafViewVTKButtons() {
}

mafResources::mafSceneNode *mafViewVTKButtons::createSceneNode(mafResources::mafVME *vme) {
    mafResources::mafSceneNode *sn = Superclass::createSceneNode(vme);
    if (sn != NULL) {
        //create the instance for selection pipe.
        mafToolVTKButtons *toolButtons = mafNEW(mafToolVTKButtons);
        toolButtons->setFollowSelectedObjectVisibility(true);
        toolButtons->setFollowSelectedObject(false);
        toolButtons->setInput(vme);
        toolButtons->setVisibility(false);
        m_ToolHandler->addTool(toolButtons);
        mafDEL(toolButtons);
    }
    return sn;
}


