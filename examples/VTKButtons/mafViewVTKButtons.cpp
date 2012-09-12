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
#include <mafVME.h>
#include <mafTool.h>
#include <mafToolHandler.h>

using namespace mafPluginVTK;

mafViewVTKButtons::mafViewVTKButtons(const QString code_location) : mafViewVTK(code_location) {
    m_UIFilename = "mafVTKButtons.ui";
}

mafViewVTKButtons::~mafViewVTKButtons() {
}

mafResources::mafSceneNode *mafViewVTKButtons::createSceneNode(mafResources::mafVME *vme) {
    mafResources::mafSceneNode *sn = Superclass::createSceneNode(vme);
    if (sn != NULL) {
        //create the instance for selection pipe.
        mafToolVTKButtons *toolButton = mafNEW(mafToolVTKButtons);
        toolButton->setVisibility(false);
        toolButton->setFollowSelectedObjectVisibility(true);
        toolButton->setFollowSelectedObject(false);
        // set the scene node because the tool even if doesn't forward a vme, it needs to keep knowledge its scenenode.
        toolButton->setSceneNode(sn);
        toolButton->setInput(vme);
        m_ToolHandler->addTool(toolButton);
        mafDEL(toolButton);

        bool res = connect(sn, SIGNAL(modifiedObject()), toolButton, SLOT(updatePipe()));
    }
    return sn;
}

void mafViewVTKButtons::removeSceneNode(mafResources::mafSceneNode *node) {
    //When the sceneNode is removed, remove also relative tool.
    if (node != NULL) {
        QList<mafToolVTKButtons *> buttonsToRemove;
        QList<mafResources::mafTool *> *tList = m_ToolHandler->toolList();

        //First: search for toolButtons inside m_ToolHandler
        for (int i = 0; i < tList->count(); i++) {
            mafResources::mafTool *tool = tList->at(i);
            mafToolVTKButtons *button = dynamic_cast<mafToolVTKButtons *>(tool);
            if (button && button->input() && button->input()->isEqual(node->vme())) {
                buttonsToRemove.append(button);
            }
        }

        //Then: remove toolButtons from m_ToolHandler
        for (int i = 0; i < buttonsToRemove.count(); i++) {
            mafToolVTKButtons *button = buttonsToRemove.at(i);
            button->resetTool();
            m_ToolHandler->removeTool(button);
        }
    }
    Superclass::removeSceneNode(node);
}

void mafViewVTKButtons::clearScene() {
    QList<mafToolVTKButtons *> buttonsToRemove;
    QList<mafResources::mafTool *> *tList = m_ToolHandler->toolList();

    //First: search for toolButtons inside m_ToolHandler
    for (int i = 0; i < tList->count(); i++) {
        mafResources::mafTool *tool = tList->at(i);
        mafToolVTKButtons *button = dynamic_cast<mafToolVTKButtons *>(tool);
        if(button) {
            buttonsToRemove.append(button);
        }
    }

    //Then: remove toolButtons from m_ToolHandler
    for (int i = 0; i < buttonsToRemove.count(); i++) {
        mafToolVTKButtons *button = buttonsToRemove.at(i);
        button->resetTool();
        m_ToolHandler->removeTool(button);
    }
    Superclass::clearScene();
}

void mafViewVTKButtons::on_showButtonsCheck_stateChanged(int state) {
    //Update all mafToolVTKButtons
    QList<mafResources::mafTool *> *tList = m_ToolHandler->toolList();
    for (int i = 0; i < tList->count(); i++) {
        mafResources::mafTool *tool = tList->at(i);
        mafToolVTKButtons *button = dynamic_cast<mafToolVTKButtons *>(tool);
        if(button) {
            //need to switch off setFollowSelectedObjectVisibility if showButtonsCheck is false
            button->setFollowSelectedObjectVisibility(state);
            if (state) {
                //when showButtons is set to ON, I have to show only buttons of visible SceneNodes
                if (button->sceneNode()) {
                    bool visibility = button->sceneNode()->property("visibility").toBool();
                    button->setVisibility(visibility);
                }
            } else {
                button->setVisibility(state);
            }
            button->updatePipe();
        }
    }
}

void mafViewVTKButtons::on_showLabelsCheck_stateChanged(int state) {
    //Update all mafToolVTKButtons
    QList<mafResources::mafTool *> *tList = m_ToolHandler->toolList();
    for (int i = 0; i < tList->count(); i++) {
        mafResources::mafTool *tool = tList->at(i);
        mafToolVTKButtons *button = dynamic_cast<mafToolVTKButtons *>(tool);
        if(button) {
            button->setShowLabel(state);
            button->updatePipe();
        }
    }
}

void mafViewVTKButtons::on_flyToCheck_stateChanged(int state) {
    //Update all mafToolVTKButtons
    QList<mafResources::mafTool *> *tList = m_ToolHandler->toolList();
    for (int i = 0; i < tList->count(); i++) {
        mafResources::mafTool *tool = tList->at(i);
        mafToolVTKButtons *button = dynamic_cast<mafToolVTKButtons *>(tool);
        if(button) {
            button->setFlyTo(state);
            button->updatePipe();
        }
    }
}

void mafViewVTKButtons::on_positionComboBox_activated(int state) {
    //Update all mafToolVTKButtons
    QList<mafResources::mafTool *> *tList = m_ToolHandler->toolList();
    for (int i = 0; i < tList->count(); i++) {
        mafResources::mafTool *tool = tList->at(i);
        mafToolVTKButtons *button = dynamic_cast<mafToolVTKButtons *>(tool);
        if(button) {
            button->setOnCenter(state);
            button->updatePipe();
        }
    }
}


