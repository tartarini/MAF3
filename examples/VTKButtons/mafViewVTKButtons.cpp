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

#include <mafToolHandler.h>

using namespace mafPluginVTK;

mafViewVTKButtons::mafViewVTKButtons(const QString code_location) : mafViewVTK(code_location) {
}

mafViewVTKButtons::~mafViewVTKButtons() {
}

bool mafViewVTKButtons::initialize() {
    if (Superclass::initialize()) {
 
        //create the instance for selection pipe.
        mafToolVTKButtons *toolButtons = mafNEW(mafToolVTKButtons);
        //toolButtons->setFollowSelectedObjectVisibility(true);
        //toolButtons->setFollowSelectedObject(true);
        m_ToolHandler->addTool(toolButtons);
        mafDEL(toolButtons);
       
        return true;
    }
    return false;
}

