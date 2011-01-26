/*
 *  mafVMEManager.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVMEManager.h"
#include "mafVME.h"

using namespace mafCore;
using namespace mafResources;

mafVMEManager* mafVMEManager::instance() {
    static mafVMEManager instanceVMEManager;
    return &instanceVMEManager;
}

void mafVMEManager::shutdown() {
}

mafVMEManager::mafVMEManager(const mafString code_location) : mafObjectBase(code_location), m_SelectedVME(NULL), m_Root(NULL), m_VMEHierarchy(NULL) {
    initializeConnections();
}

mafVMEManager::~mafVMEManager() {
    //m_VMEHierarchy->clear();
    mafDEL(m_Root);
    mafDEL(m_VMEHierarchy);
}

void mafVMEManager::initializeConnections() {
    // Create the IDs required to add a resource to the management system.
    mafIdProvider *provider = mafIdProvider::instance();
    provider->createNewId("maf.local.resources.vme.add");
    provider->createNewId("maf.local.resources.vme.remove");
    provider->createNewId("maf.local.resources.vme.select");
    provider->createNewId("maf.local.resources.vme.selected");
    provider->createNewId("maf.local.resources.hierarchy.create");

    // Register API signals.
    mafRegisterLocalSignal("maf.local.resources.vme.add", this, "attachVMEToHierarchy(mafCore::mafObjectBase *)");
    mafRegisterLocalSignal("maf.local.resources.vme.remove", this, "detachVMEFromHierarchy(mafCore::mafObjectBase *)");
    mafRegisterLocalSignal("maf.local.resources.vme.select", this, "selectVME(mafCore::mafObjectBase *)");
    mafRegisterLocalSignal("maf.local.resources.vme.selected", this, "selectedVMESignal()");
    mafRegisterLocalSignal("maf.local.resources.hierarchy.create", this, "createVMEHierarchySignal()");

    // Register private callbacks to the instance of the manager..
    mafRegisterLocalCallback("maf.local.resources.vme.add", this, "vmeAdd(mafCore::mafObjectBase *)");
    mafRegisterLocalCallback("maf.local.resources.vme.remove", this, "vmeRemove(mafCore::mafObjectBase *)");
    mafRegisterLocalCallback("maf.local.resources.vme.select", this, "vmeSelect(mafCore::mafObjectBase *)");
    mafRegisterLocalCallback("maf.local.resources.vme.selected", this, "selectedVME()");
    mafRegisterLocalCallback("maf.local.resources.hierarchy.create", this, "createVMEHierarchy()");
}

void mafVMEManager::vmeSelect(mafObjectBase *vme) {
    mafVME *vme_to_select = dynamic_cast<mafResources::mafVME *>(vme);
    if(NULL == vme_to_select) {
        mafMsgWarning("%s", mafTr("Trying to select an object that not represent a mafVME.").toAscii().data());
        return;
    }
    // VME has been selected.
    m_SelectedVME = vme_to_select;
}

void mafVMEManager::vmeAdd(mafObjectBase *vme) {
    mafVME *vme_to_add = dynamic_cast<mafResources::mafVME *>(vme);
    if(NULL == vme_to_add) {
        mafMsgWarning("%s", mafTr("Trying to add an object that not represent a mafVME.").toAscii().data());
        return;
    }
    // VME has been added.
    // Connect the manager to the view destroyed signal
    connect(vme_to_add, SIGNAL(destroyed()), this, SLOT(vmeDestroyed()));
}

void mafVMEManager::vmeRemove(mafObjectBase *vme) {
    mafVME *vme_to_remove = dynamic_cast<mafResources::mafVME *>(vme);
    if(NULL == vme_to_remove) {
        mafMsgWarning("%s", mafTr("Trying to remove an object that not represent a mafVME.").toAscii().data());
        return;
    }
    // VME has been removed.
    vme_to_remove->detatch();
    removeVME(vme_to_remove);
}

void mafVMEManager::vmeDestroyed() {
    mafVME *vme = (mafVME *)QObject::sender();
    removeVME(vme);
}

void mafVMEManager::removeVME(mafVME *vme) {
    // Disconnect the manager from the vme
    disconnect(vme, SIGNAL(destroyed()),this, SLOT(vmeDestroyed()));
    // remove the VME from the managed resources and manage the active resource if the removed VME is the active one.
    if (vme == m_SelectedVME) {
        m_SelectedVME = NULL;
    }
}

mafCore::mafHierarchyPointer mafVMEManager::createVMEHierarchy() {
     if(m_VMEHierarchy == NULL) {
         m_VMEHierarchy = mafNEW(mafCore::mafHierarchy);

         //Delete previous created root.
         mafDEL(m_Root);
         //Create a new root.
         m_Root = mafNEW(mafResources::mafVME);
         m_Root->setObjectName("root");

         m_VMEHierarchy->addHierarchyNode(m_Root);
     }

     return m_VMEHierarchy;
}
