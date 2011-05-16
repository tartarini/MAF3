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

mafInteractionManager::mafInteractionManager(const QString code_location) : mafObjectBase(code_location) {
    initializeConnections();
}

mafInteractionManager::~mafInteractionManager() {
}

void mafInteractionManager::initializeConnections() {
    // Register API signals.
    mafRegisterLocalSignal("maf.local.resources.interaction.leftButtonPress", this, "leftButtonPressSignal(unsigned long)");
    mafRegisterLocalSignal("maf.local.resources.interaction.leftButtonRelease", this, "leftButtonReleaseSignal(unsigned long)");
    mafRegisterLocalSignal("maf.local.resources.interaction.middleButtonPress", this, "middleButtonPressSignal(unsigned long)");
    mafRegisterLocalSignal("maf.local.resources.interaction.middleButtonRelease", this, "middleButtonReleaseSignal(unsigned long)");
    mafRegisterLocalSignal("maf.local.resources.interaction.rightButtonPress", this, "rightButtonPressSignal(unsigned long)");
    mafRegisterLocalSignal("maf.local.resources.interaction.rightButtonRelease", this, "rightButtonReleaseSignal(unsigned long)");
    mafRegisterLocalSignal("maf.local.resources.interaction.enter", this, "enterSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.leave", this, "leaveSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.keyPress", this, "keyPressSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.keyRelease", this, "keyReleaseSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.keyChar", this, "charSignal()");
    mafRegisterLocalSignal("maf.local.resources.interaction.mouseMove", this, "mouseMoveSignal(unsigned long)");
    mafRegisterLocalSignal("maf.local.resources.interaction.mouseWheelForward", this, "mouseWheelForwardSignal(unsigned long)");
    mafRegisterLocalSignal("maf.local.resources.interaction.mouseWheelBackward", this, "mouseWheelBackwardSignal(unsigned long)");
    mafRegisterLocalSignal("maf.local.resources.interaction.pick", this, "pickSignal()");

    // Register private callbacks.
    mafRegisterLocalCallback("maf.local.resources.interaction.leftButtonPress", this, "leftButtonPress(unsigned long)");
    mafRegisterLocalCallback("maf.local.resources.interaction.leftButtonRelease", this, "leftButtonRelease(unsigned long)");
    mafRegisterLocalCallback("maf.local.resources.interaction.middleButtonPress", this, "middleButtonPress(unsigned long)");
    mafRegisterLocalCallback("maf.local.resources.interaction.middleButtonRelease", this, "middleButtonRelease(unsigned long)");
    mafRegisterLocalCallback("maf.local.resources.interaction.rightButtonPress", this, "rightButtonPress(unsigned long)");
    mafRegisterLocalCallback("maf.local.resources.interaction.rightButtonRelease", this, "rightButtonRelease(unsigned long)");
    mafRegisterLocalCallback("maf.local.resources.interaction.enter", this, "enter()");
    mafRegisterLocalCallback("maf.local.resources.interaction.leave", this, "leave()");
    mafRegisterLocalCallback("maf.local.resources.interaction.keyPress", this, "keyPress()");
    mafRegisterLocalCallback("maf.local.resources.interaction.keyRelease", this, "keyRelease()");
    mafRegisterLocalCallback("maf.local.resources.interaction.keyChar", this, "keyChar()");
    mafRegisterLocalCallback("maf.local.resources.interaction.mouseMove", this, "mouseMove(unsigned long)");
    mafRegisterLocalCallback("maf.local.resources.interaction.mouseWheelForward", this, "mouseWheelForward(unsigned long)");
    mafRegisterLocalCallback("maf.local.resources.interaction.mouseWheelBackward", this, "mouseWheelBackward(unsigned long)");
    mafRegisterLocalCallback("maf.local.resources.interaction.pick", this, "pick()");
}

void mafInteractionManager::vmeSelected(mafVME *vme) {
    Q_UNUSED(vme);
}

void mafInteractionManager::leftButtonPress(unsigned long modifier) {
}

void mafInteractionManager::leftButtonRelease(unsigned long modifier) {
}

void mafInteractionManager::middleButtonPress(unsigned long modifier) {
}

void mafInteractionManager::middleButtonRelease(unsigned long modifier) {
}

void mafInteractionManager::rightButtonPress(unsigned long modifier) {
}

void mafInteractionManager::rightButtonRelease(unsigned long modifier) {
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

void mafInteractionManager::mouseMove(unsigned long modifier) {
}

void mafInteractionManager::mouseWheelForward(unsigned long modifier) {
}

void mafInteractionManager::mouseWheelBackward(unsigned long modifier) {
}

void mafInteractionManager::pick() {
}

