/*
 *  mafVisualPipe.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVisualPipe.h"
#include "mafVME.h"
#include <mafContainerInterface.h>
#include <mafEventBusManager.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;


mafVisualPipe::mafVisualPipe(const mafString code_location) : mafPipe(code_location), m_Output(NULL) {
    initializeConnections();
}

mafVisualPipe::~mafVisualPipe() {
    mafEventBusManager::instance()->removeSignal(this, "maf.local.resources.interaction.vmePick");
    mafEventBusManager::instance()->removeSignal(this, "maf.local.resources.interaction.vmePicked");
}

void mafVisualPipe::initializeConnections() {

    //mafId vme_picked_id = mafIdProvider::instance()->idValue("maf.local.resources.interaction.vmePick");
    //mafIdProvider::instance()->createNewId("maf.local.resources.interaction.vmePick");

    // Register API signals.
    mafRegisterLocalSignal("maf.local.resources.interaction.vmePick", this, "vmePickSignal(double *, unsigned long, mafCore::mafContainerInterface *, QEvent *)");

    // Register private callbacks.
    mafRegisterLocalCallback("maf.local.resources.interaction.vmePick", this, "vmePick(double *, unsigned long, mafCore::mafContainerInterface *, QEvent *)");

    // Register API signals.
    mafRegisterLocalSignal("maf.local.resources.interaction.vmePicked", this, "vmePickedSignal(double *, unsigned long, mafCore::mafObjectBase *)");
}

void mafVisualPipe::vmePick(double *pickPos, unsigned long modifiers, mafCore::mafContainerInterface *actor, QEvent * e) {
    Q_UNUSED(e);
    if (m_Output->isEqual(actor)){
        mafVME *vme = this->inputList()->at(0);
        if (vme != NULL){
            mafEventArgumentsList argList;
            argList.append(mafEventArgument(double *, pickPos));
            argList.append(mafEventArgument(unsigned long, modifiers));
            argList.append(mafEventArgument(mafCore::mafObjectBase *, vme));
            mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.vmePicked", mafEventTypeLocal, &argList);
        }
    }
}

void mafVisualPipe::setVisibility(bool visible) {
    m_Visibility = visible;
}
