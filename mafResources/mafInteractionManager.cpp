/*
 *  mafInteractionManager.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafInteractionManager.h"

using namespace mafCore;
using namespace mafResources;

mafInteractionManager* mafInteractionManager::instance() {
    // Create the instance of the Interaction Manager manager.
    static mafInteractionManager instanceInteractionManager;
    return &instanceInteractionManager;
}

void mafInteractionManager::shutdown() {
}

mafInteractionManager::mafInteractionManager(const mafString code_location) : mafObjectBase(code_location) {
    initializeConnections();
}

mafInteractionManager::~mafInteractionManager() {
}

void mafInteractionManager::initializeConnections() {
    // Register API signals.
    mafRegisterLocalSignal("maf.local.resources.interaction.leftButtonPress", this, "leftButtonPressSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.leftButtonRelease", this, "leftButtonReleaseSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.middleButtonPress", this, "middleButtonPressSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.middleButtonRelease", this, "middleButtonReleaseSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.rightButtonPress", this, "rightButtonPressSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.rightButtonRelease", this, "rightButtonReleaseSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.enter", this, "enterSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.leave", this, "leaveSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.keyPress", this, "keyPressSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.keyRelease", this, "keyReleaseSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.keyChar", this, "charSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.mouseMove", this, "mouseMoveSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.mouseWheelForward", this, "mouseWheelForwardSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.mouseWheelBackward", this, "mouseWheelBackwardSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.pick", this, "pickSignal()");

    // Register private callbacks.
    mafRegisterLocalCallback("maf.local.resources.interaction.leftButtonPress", this, "leftButtonPress()");
    mafRegisterLocalCallback("maf.local.resources.interaction.leftButtonRelease", this, "leftButtonRelease()");
    mafRegisterLocalCallback("maf.local.resources.interaction.middleButtonPress", this, "middleButtonPress()");
    mafRegisterLocalCallback("maf.local.resources.interaction.middleButtonRelease", this, "middleButtonRelease()");
    mafRegisterLocalCallback("maf.local.resources.interaction.rightButtonPress", this, "rightButtonPress()");
    mafRegisterLocalCallback("maf.local.resources.interaction.rightButtonRelease", this, "rightButtonRelease()");
    mafRegisterLocalCallback("maf.local.resources.interaction.enter", this, "enter()");
    mafRegisterLocalCallback("maf.local.resources.interaction.leave", this, "leave()");
    mafRegisterLocalCallback("maf.local.resources.interaction.keyPress", this, "keyPress()");
    mafRegisterLocalCallback("maf.local.resources.interaction.keyRelease", this, "keyRelease()");
    mafRegisterLocalCallback("maf.local.resources.interaction.keyChar", this, "keyChar()");
    mafRegisterLocalCallback("maf.local.resources.interaction.mouseMove", this, "mouseMove()");
    mafRegisterLocalCallback("maf.local.resources.interaction.mouseWheelForward", this, "mouseWheelForward()");
    mafRegisterLocalCallback("maf.local.resources.interaction.mouseWheelBackward", this, "mouseWheelBackward()");
    mafRegisterLocalCallback("maf.local.resources.interaction.pick", this, "pick()");
}

void mafInteractionManager::vmeSelected(mafVME *vme) {

}

void mafInteractionManager::leftButtonPress() {
    mafMsgDebug() << "left mouse button pressed!!";
}

void mafInteractionManager::leftButtonRelease() {
}

void mafInteractionManager::middleButtonPress() {
}

void mafInteractionManager::middleButtonRelease() {
}

void mafInteractionManager::rightButtonPress() {
}

void mafInteractionManager::rightButtonRelease() {
}

void mafInteractionManager::enter() {
}

void mafInteractionManager::leave() {
}

void mafInteractionManager::keyPress() {
}

void mafInteractionManager::keyRelease() {
}

void mafInteractionManager::keyChar() {
}

void mafInteractionManager::mouseMove() {
}

void mafInteractionManager::mouseWheelForward() {
}

void mafInteractionManager::mouseWheelBackward() {
}

void mafInteractionManager::pick() {
}

