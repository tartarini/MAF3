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
}

void mafInteracionManager::vmeSelected(mafVME *vme) {

}
