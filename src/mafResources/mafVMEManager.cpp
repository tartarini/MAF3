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
#include "mafSceneNode.h"

using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

mafVMEManager* mafVMEManager::instance() {
    static mafVMEManager instanceVMEManager;
    return &instanceVMEManager;
}

void mafVMEManager::shutdown() {
    m_VMEHierarchy->clear();
    mafDEL(m_Root);
    m_SelectedVME = NULL;
}

mafVMEManager::mafVMEManager(const QString code_location) : mafObjectBase(code_location), m_SelectedVME(NULL), m_Root(NULL), m_VMEHierarchy(NULL) {
    initializeConnections();
    requestVMEHierarchy();
}

mafVMEManager::~mafVMEManager() {
    shutdown();
    mafDEL(m_VMEHierarchy);
    
    // Unregister callbacks...
    mafUnregisterLocalCallback("maf.local.resources.vme.add", this, "vmeAdd(mafCore::mafObjectBase *)")
    mafUnregisterLocalCallback("maf.local.resources.vme.remove", this, "vmeRemove(mafCore::mafObjectBase *)")
    mafUnregisterLocalCallback("maf.local.resources.vme.reparent", this, "vmeReparent(mafCore::mafObjectBase *, mafCore::mafObjectBase *)")
    mafUnregisterLocalCallback("maf.local.resources.vme.select", this, "vmeSelect(mafCore::mafObjectBase *)")
    mafUnregisterLocalCallback("maf.local.resources.vme.selected", this, "selectedVME()")
    mafUnregisterLocalCallback("maf.local.resources.hierarchy.request", this, "requestVMEHierarchy()")
    
    // Unregister signals...
    mafUnregisterLocalSignal("maf.local.resources.vme.add", this, "attachVMEToHierarchy(mafCore::mafObjectBase *)")
    mafUnregisterLocalSignal("maf.local.resources.vme.remove", this, "detachVMEFromHierarchy(mafCore::mafObjectBase *)")
    mafUnregisterLocalSignal("maf.local.resources.vme.reparent", this, "reparentVMESignal(mafCore::mafObjectBase *, mafCore::mafObjectBase *)")
    mafUnregisterLocalSignal("maf.local.resources.vme.select", this, "selectVME(mafCore::mafObjectBase *)")
    mafUnregisterLocalSignal("maf.local.resources.vme.selected", this, "selectedVMESignal()")
    mafUnregisterLocalSignal("maf.local.resources.hierarchy.request", this, "requestVMEHierarchySignal()")

    // Remove IDs.
    mafIdProvider *provider = mafIdProvider::instance();
    provider->removeId("maf.local.resources.vme.add");
    provider->removeId("maf.local.resources.vme.remove");
    provider->removeId("maf.local.resources.vme.reparent");
    provider->removeId("maf.local.resources.vme.select");
    provider->removeId("maf.local.resources.vme.selected");
    provider->removeId("maf.local.resources.hierarchy.request");
}

void mafVMEManager::initializeConnections() {
    // Create the IDs required to add a resource to the management system.
    mafIdProvider *provider = mafIdProvider::instance();
    provider->createNewId("maf.local.resources.vme.add");
    provider->createNewId("maf.local.resources.vme.remove");
    provider->createNewId("maf.local.resources.vme.reparent");
    provider->createNewId("maf.local.resources.vme.select");
    provider->createNewId("maf.local.resources.vme.selected");
    provider->createNewId("maf.local.resources.hierarchy.request");

    // Register API signals.
    mafRegisterLocalSignal("maf.local.resources.vme.add", this, "attachVMEToHierarchy(mafCore::mafObjectBase *)")
    mafRegisterLocalSignal("maf.local.resources.vme.remove", this, "detachVMEFromHierarchy(mafCore::mafObjectBase *)")
    mafRegisterLocalSignal("maf.local.resources.vme.reparent", this, "reparentVMESignal(mafCore::mafObjectBase *, mafCore::mafObjectBase *)")
    mafRegisterLocalSignal("maf.local.resources.vme.select", this, "selectVME(mafCore::mafObjectBase *)")
    mafRegisterLocalSignal("maf.local.resources.vme.selected", this, "selectedVMESignal()")
    mafRegisterLocalSignal("maf.local.resources.hierarchy.request", this, "requestVMEHierarchySignal()")

    // Register private callbacks to the instance of the manager..
    mafRegisterLocalCallback("maf.local.resources.vme.add", this, "vmeAdd(mafCore::mafObjectBase *)")
    mafRegisterLocalCallback("maf.local.resources.vme.remove", this, "vmeRemove(mafCore::mafObjectBase *)")
    mafRegisterLocalCallback("maf.local.resources.vme.reparent", this, "vmeReparent(mafCore::mafObjectBase *, mafCore::mafObjectBase *)")
    mafRegisterLocalCallback("maf.local.resources.vme.select", this, "vmeSelect(mafCore::mafObjectBase *)")
    mafRegisterLocalCallback("maf.local.resources.vme.selected", this, "selectedVME()")
    mafRegisterLocalCallback("maf.local.resources.hierarchy.request", this, "requestVMEHierarchy()")
}

void mafVMEManager::vmeSelect(mafObjectBase *vme) {
    mafVME *vme_to_select = qobject_cast<mafResources::mafVME *>(vme);
    if(NULL == vme_to_select) {
        qWarning("%s", mafTr("Trying to select an object that not represent a mafVME.").toAscii().data());
        return;
    }
    // VME has been selected.
    if(m_SelectedVME) {
        m_SelectedVME->setSelected(false);
    }
    m_SelectedVME = vme_to_select;
}

void mafVMEManager::vmeAdd(mafObjectBase *vme) {
    mafVME *vme_to_add = qobject_cast<mafResources::mafVME *>(vme);
    if(NULL == vme_to_add) {
        qWarning("%s", mafTr("Trying to add an object that not represent a mafVME.").toAscii().data());
        return;
    }
    // VME has been added.
    connect(vme, SIGNAL(destroyed()), this, SLOT(vmeDestroyed()), Qt::DirectConnection);
    m_VMEHierarchy->addHierarchyNode(vme, m_SelectedVME);
}

void mafVMEManager::vmeRemove(mafObjectBase *vme) {
    mafVME *vme_to_remove = qobject_cast<mafResources::mafVME *>(vme);
    if(NULL == vme_to_remove) {
        qWarning("%s", mafTr("Trying to remove an object that not represent a mafVME.").toAscii().data());
        return;
    }
    // VME has been removed.
    vme_to_remove->detatchFromTree();
//    removeVME(vme_to_remove);

    m_VMEHierarchy->removeHierarchyNode(vme);
}

void mafVMEManager::vmeReparent(mafObjectBase *vme, mafObjectBase *vmeParent) {
  if (m_VMEHierarchy != NULL) {
    QString objName = "mafResources::mafSceneNode";
    if (objName.compare(vme->metaObject()->className()) == 0) {
      mafSceneNode *obj = qobject_cast<mafSceneNode *>(vme);
      mafSceneNode *objParent = qobject_cast<mafSceneNode *>(vmeParent);
      m_VMEHierarchy->reparentHierarchyNode(obj->vme(), objParent->vme());
    } else {
        m_VMEHierarchy->reparentHierarchyNode(vme, vmeParent);
    }
  }
}


void mafVMEManager::vmeDestroyed() {
    mafVME *vme = (mafVME *)QObject::sender();
    if ( vme == m_SelectedVME ) {
        m_SelectedVME = NULL;
    }
}

//void mafVMEManager::removeVME(mafVME *vme) {
    // Disconnect the manager from the vme
//    disconnect(vme, SIGNAL(destroyed()),this, SLOT(vmeDestroyed()));
    // remove the VME from the managed resources and manage the active resource if the removed VME is the active one.
//    if ( vme == m_SelectedVME ) {
//        m_SelectedVME = NULL;
//    }
    //m_VMEHierarchy->removeHierarchyNode(vme); // DEPRECATED
//}

mafCore::mafHierarchyPointer mafVMEManager::requestVMEHierarchy() {
     if ( m_VMEHierarchy == NULL ) {
         m_VMEHierarchy = mafNEW(mafCore::mafHierarchy);
    }

    if (m_Root == NULL) {
         //Create a new root.
         m_Root = mafNEW(mafResources::mafVME);
         m_Root->setObjectName("root");
         m_Root->setProperty("iconFile", ":/images/root.png");

         m_VMEHierarchy->addHierarchyNode(m_Root);

         // Select the root node.
         m_Root->setSelected(true);
     }

     return m_VMEHierarchy;
}
