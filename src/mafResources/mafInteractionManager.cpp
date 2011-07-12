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
#include "mafVME.h"
#include "mafInteractor.h"

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
    initialize();
}

mafInteractionManager::~mafInteractionManager() {
}

void mafInteractionManager::initialize() {
}

void mafInteractionManager::buttonPress(double *pos, unsigned long modifiers, QEvent *e) {
    //PRINT_FUNCTION_NAME_INFORMATION
}

void mafInteractionManager::buttonRelease(double *pos, unsigned long modifiers, QEvent *e) {
    //PRINT_FUNCTION_NAME_INFORMATION
}


void mafInteractionManager::mouseMove(double *pos, unsigned long modifiers, QEvent *e) {
    //PRINT_FUNCTION_NAME_INFORMATION
}

void mafInteractionManager::enter(unsigned long modifiers, QEvent *e) {
    //PRINT_FUNCTION_NAME_INFORMATION
}

void mafInteractionManager::leave(unsigned long modifiers, QEvent *e) {
    //PRINT_FUNCTION_NAME_INFORMATION
}

void mafInteractionManager::keyPress(QEvent *e) {
    //PRINT_FUNCTION_NAME_INFORMATION
}

void mafInteractionManager::keyRelease(QEvent *e) {
    //PRINT_FUNCTION_NAME_INFORMATION
}


void mafInteractionManager::mouseWheelForward(unsigned long modifiers, QEvent *e) {
    //PRINT_FUNCTION_NAME_INFORMATION
}

void mafInteractionManager::mouseWheelBackward(unsigned long modifiers, QEvent *e) {
   // PRINT_FUNCTION_NAME_INFORMATION
}

void mafInteractionManager::vmePicked(double *pos, unsigned long modifiers, mafVME *vme, QEvent *e) {
    mafInteractor *i = vme->activeInteractor();
    if(i) {
        // execute the interactor
        i->vmePicked(pos, modifiers, vme, e);
        return;
    } else {
        //default behaviour
    }
}

