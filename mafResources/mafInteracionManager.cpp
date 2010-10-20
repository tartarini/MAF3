/*
 *  mafInteracionManager.m
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafInteracionManager.h"

using namespace mafCore;
using namespace mafResources;

mafInteracionManager* mafInteracionManager::instance() {
    // Create the instance of the VME manager.
    static mafInteracionManager instanceInteractionManager;
    return &instanceInteractionManager;
}

void mafInteracionManager::shutdown() {
}

mafInteracionManager::mafInteracionManager(const mafString code_location) : mafObjectBase(code_location) {
    initializeConnections();
}

mafInteracionManager::~mafInteracionManager() {
}

void mafInteracionManager::initializeConnections() {
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
    mafRegisterLocalSignal("maf.local.resources.interaction.char", this, "charSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.timer", this, "timerSignal()");
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
    mafRegisterLocalCallback("maf.local.resources.interaction.char", this, "char()");
    mafRegisterLocalCallback("maf.local.resources.interaction.timer", this, "timer()");
    mafRegisterLocalCallback("maf.local.resources.interaction.mouseMove", this, "mouseMove()");
    mafRegisterLocalCallback("maf.local.resources.interaction.mouseWheelForward", this, "mouseWheelForward()");
    mafRegisterLocalCallback("maf.local.resources.interaction.mouseWheelBackward", this, "mouseWheelBackward()");
    mafRegisterLocalCallback("maf.local.resources.interaction.pick", this, "pick()");
}

void mafInteracionManager::vmeSelected(mafVME *vme) {

}

void mafInteracionManager::leftButtonPress() {
    mafMsgDebug() << "left mouse button pressed!!";
}

void mafInteracionManager::leftButtonRelease() {
}

void mafInteracionManager::middleButtonPress() {
}

void mafInteracionManager::middleButtonRelease() {
}

void mafInteracionManager::rightButtonPress() {
}

void mafInteracionManager::rightButtonRelease() {
}

void mafInteracionManager::enter() {
}

void mafInteracionManager::leave() {
}

void mafInteracionManager::keyPress() {
}

void mafInteracionManager::keyRelease() {
}

void mafInteracionManager::char() {
}

void mafInteracionManager::timer() {
}

void mafInteracionManager::mouseMove() {
}

void mafInteracionManager::mouseWheelForward() {
}

void mafInteracionManager::mouseWheelBackward() {
}

void mafInteracionManager::pick() {
}

