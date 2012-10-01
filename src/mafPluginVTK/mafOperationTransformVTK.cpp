/*
 *  mafOperationTransformVTK.cpp
 *  mafResources
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 13/07/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafOperationTransformVTK.h"
#include "mafInteractorVTKTransform.h"
#include "mafVME.h"

using namespace mafResources;
using namespace mafPluginVTK;

mafOperationTransformVTK::mafOperationTransformVTK(const QString code_location) : mafOperationTransform(code_location) {
    setObjectName("mafOperationTransformVTK");
    
    //set multithreaded to off
    m_MultiThreaded = false;
}

mafOperationTransformVTK::~mafOperationTransformVTK() {
}

bool mafOperationTransformVTK::acceptObject(mafCore::mafObjectBase *obj) {
    return true;
}

bool mafOperationTransformVTK::initialize() {
    bool result = Superclass::initialize();
    
    mafInteractorVTKTransform *interactor = mafNEW(mafPluginVTK::mafInteractorVTKTransform);
    mafVME *vme = qobject_cast<mafVME *>(input());
    vme->pushInteractor(interactor);
    connect(interactor, SIGNAL(interactingSignal()), this, SLOT(updateFromInteraction()));
    mafDEL(interactor);
    
    return result;
}

void mafOperationTransformVTK::terminated() {
    mafVME *vme = qobject_cast<mafVME *>(input());
    vme->popInteractor();
    
    Superclass::terminated();
}

