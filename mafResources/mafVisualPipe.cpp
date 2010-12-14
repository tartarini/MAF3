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
#include <mafEventBusManager.h>
#include "mafVME.h"

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;


mafVisualPipe::mafVisualPipe(const mafString code_location) : mafPipe(code_location), m_Output(NULL) {
    initializeConnections();
}

mafVisualPipe::~mafVisualPipe() {
}

void mafVisualPipe::initializeConnections() {

    mafId vme_picked_id = mafIdProvider::instance()->idValue("maf.local.resources.interaction.vmeSelect");
    if(vme_picked_id == -1) {
        mafIdProvider::instance()->createNewId("maf.local.resources.interaction.vmeSelect");

        // Register API signals.
        mafRegisterLocalSignal("maf.local.resources.interaction.vmeSelect", this, "vmeSelectSignal(double *, unsigned long, mafCore::mafContainerInterface *)");

        // Register private callbacks.
        mafRegisterLocalCallback("maf.local.resources.interaction.vmeSelect", this, "vmeSelect(double *, unsigned long, mafCore::mafContainerInterface *)");
    }
}

void mafVisualPipe::vmeSelect(double *selectPos, unsigned long modifiers, mafCore::mafContainerInterface *actor) {
    if(actor == m_Output){
        mafVME *vme = this->inputList()->at(0);
        if (vme != NULL){
            mafEventArgumentsList argList;
            argList.append(mafEventArgument(double *, selectPos));
            argList.append(mafEventArgument(unsigned long, modifiers));
            argList.append(mafEventArgument(mafCore::mafObjectBase *, vme));
            mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.vmeSelected", mafEventTypeLocal, &argList);
        }
    }

}
