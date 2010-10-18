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
    mafRegisterLocalSignal("maf.local.resources.VTK.interaction.leftButtonPress", this, "leftButtonPressVTKSignal()");
    mafRegisterLocalSignal("maf.local.resources.VTK.interaction.leftButtonRelease", this, "leftButtonReleaseVTKSignal()");
    mafRegisterLocalSignal("maf.local.resources.VTK.interaction.middleButtonPress", this, "middleButtonPressVTKSignal()");
    mafRegisterLocalSignal("maf.local.resources.VTK.interaction.middleButtonRelease", this, "middleButtonReleaseVTKSignal()");
    mafRegisterLocalSignal("maf.local.resources.VTK.interaction.rightButtonPress", this, "rightButtonPressVTKSignal()");
    mafRegisterLocalSignal("maf.local.resources.VTK.interaction.rightButtonRelease", this, "rightButtonReleaseVTKSignal()");
    mafRegisterLocalSignal("maf.local.resources.VTK.interaction.enter", this, "enterVTKSignal()");
    mafRegisterLocalSignal("maf.local.resources.VTK.interaction.leave", this, "leaveVTKSignal()");
    mafRegisterLocalSignal("maf.local.resources.VTK.interaction.keyPress", this, "keyPressVTKSignal()");
    mafRegisterLocalSignal("maf.local.resources.VTK.interaction.keyRelease", this, "keyReleaseVTKSignal()");
    mafRegisterLocalSignal("maf.local.resources.VTK.interaction.char", this, "charVTKSignal()");
    mafRegisterLocalSignal("maf.local.resources.VTK.interaction.timer", this, "timerVTKSignal()");
    mafRegisterLocalSignal("maf.local.resources.VTK.interaction.mouseMove", this, "mouseMoveVTKSignal()");
    mafRegisterLocalSignal("maf.local.resources.VTK.interaction.mouseWheelForward", this, "mouseWheelForwardVTKSignal()");
    mafRegisterLocalSignal("maf.local.resources.VTK.interaction.mouseWheelBackward", this, "mouseWheelBackwardVTKSignal()");
    mafRegisterLocalSignal("maf.local.resources.VTK.interaction.pick", this, "pickVTKSignal()");

    // Register private callbacks.
    mafRegisterLocalCallback("maf.local.resources.VTK.interaction.leftButtonPress", this, "leftButtonPress()");
    mafRegisterLocalCallback("maf.local.resources.VTK.interaction.leftButtonRelease", this, "leftButtonRelease()");
    mafRegisterLocalCallback("maf.local.resources.VTK.interaction.middleButtonPress", this, "middleButtonPress()");
    mafRegisterLocalCallback("maf.local.resources.VTK.interaction.middleButtonRelease", this, "middleButtonRelease()");
    mafRegisterLocalCallback("maf.local.resources.VTK.interaction.rightButtonPress", this, "rightButtonPress()");
    mafRegisterLocalCallback("maf.local.resources.VTK.interaction.rightButtonRelease", this, "rightButtonRelease()");
    mafRegisterLocalCallback("maf.local.resources.VTK.interaction.enter", this, "enter()");
    mafRegisterLocalCallback("maf.local.resources.VTK.interaction.leave", this, "leave()");
    mafRegisterLocalCallback("maf.local.resources.VTK.interaction.keyPress", this, "keyPress()");
    mafRegisterLocalCallback("maf.local.resources.VTK.interaction.keyRelease", this, "keyRelease()");
    mafRegisterLocalCallback("maf.local.resources.VTK.interaction.char", this, "char()");
    mafRegisterLocalCallback("maf.local.resources.VTK.interaction.timer", this, "timer()");
    mafRegisterLocalCallback("maf.local.resources.VTK.interaction.mouseMove", this, "mouseMove()");
    mafRegisterLocalCallback("maf.local.resources.VTK.interaction.mouseWheelForward", this, "mouseWheelForward()");
    mafRegisterLocalCallback("maf.local.resources.VTK.interaction.mouseWheelBackward", this, "mouseWheelBackward()");
    mafRegisterLocalCallback("maf.local.resources.VTK.interaction.pick", this, "pick()");
}

void mafInteracionManager::vmeSelected(mafVME *vme) {

}

void mafInteracionManager::leftButtonPress() {
    mafMsgDebug() << "left mouse button pressed!!";
}
