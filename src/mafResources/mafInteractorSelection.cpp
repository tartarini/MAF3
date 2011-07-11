/*
 *  mafInteractorSelection.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 07/10/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafInteractorSelection.h"

using namespace mafCore;
using namespace mafResources;

mafInteractorSelection::mafInteractorSelection(const QString code_location) : mafInteractor(NULL, NULL, code_location) {
}

mafInteractorSelection::mafInteractorSelection(mafProxyInterfacePointer default_is, mafProxyInterfacePointer edit_is, const QString code_location) : mafInteractor(default_is, edit_is, code_location) {
}

mafInteractorSelection::~mafInteractorSelection() {
}

void mafInteractorSelection::vmePicked(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj) {
    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, obj));
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.select", mafEventBus::mafEventTypeLocal, &argList);
}
