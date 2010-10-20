/*
 *  mafViewManager.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafViewManager.h"
#include "mafMementoViewManager.h"
#include "mafView.h"
#include "mafVME.h"

using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

mafViewManager* mafViewManager::instance() {
    // Create the instance of the view manager.
    static mafViewManager instanceViewManager;
    return &instanceViewManager;
}

void mafViewManager::shutdown() {
    destroyAllViews();
}

mafViewManager::mafViewManager(const mafString code_location) : mafObjectBase(code_location), m_SelectedView(NULL) {
    initializeConnections();
}

mafViewManager::~mafViewManager() {
    destroyAllViews();
//    foreach(mafResource *v, m_CreatedViewList) {
//        mafDEL(v);
//    }
//    m_CreatedViewList.clear();
}

mafMemento *mafViewManager::createMemento() const {
    return new mafMementoViewManager(this, &m_CreatedViewList, mafCodeLocation);
}

void mafViewManager::setMemento(mafMemento *memento, bool deep_memento) {
    Q_UNUSED(deep_memento);

    mafString viewType;
    mafMementoPropertyList *list = memento->mementoPropertyList();
    mafMementoPropertyItem item;
    foreach(item, *list) {
        if(item.m_Name == "ViewType") {
            viewType = item.m_Value.toString();
            createView(viewType);
        }
    }
}

void mafViewManager::initializeConnections() {
    // Create the IDs required to add a resource to the management system.
    mafIdProvider *provider = mafIdProvider::instance();
    provider->createNewId("maf.local.resources.view.create");
    provider->createNewId("maf.local.resources.view.destroy");
    provider->createNewId("maf.local.resources.view.select");
    provider->createNewId("maf.local.resources.view.selected");
    provider->createNewId("maf.local.resources.view.vmeShow");

    // Register API signals.
    mafRegisterLocalSignal("maf.local.resources.view.create", this, "createViewSignal(mafString)");
    mafRegisterLocalSignal("maf.local.resources.view.destroy", this, "destroyViewSignal(mafCore::mafObjectBase *)");
    mafRegisterLocalSignal("maf.local.resources.view.select", this, "selectViewSignal(mafCore::mafObjectBase *)");
    mafRegisterLocalSignal("maf.local.resources.view.selected", this, "selectedViewSignal()");
    mafRegisterLocalSignal("maf.local.resources.view.vmeShow", this, "vmeShowSignal(mafCore::mafObjectBase *, bool)");

    // Register private callbacks to the instance of the manager..
    mafRegisterLocalCallback("maf.local.resources.view.create", this, "createView(mafString)");
    mafRegisterLocalCallback("maf.local.resources.view.destroy", this, "destroyView(mafCore::mafObjectBase *)");
    mafRegisterLocalCallback("maf.local.resources.view.select", this, "selectView(mafCore::mafObjectBase *)");
    mafRegisterLocalCallback("maf.local.resources.view.selected", this, "selectedView()");
    mafRegisterLocalCallback("maf.local.resources.view.vmeShow", this, "vmeShow(mafCore::mafObjectBase *, bool)");

    // Register callback to allows settings serialization.
    mafRegisterLocalCallback("maf.local.logic.settings.viewmanager.store", this, "createMemento()");
    mafRegisterLocalCallback("maf.local.logic.settings.viewmanager.restore", this, "setMemento(mafCore::mafMemento *, bool)");
}

void mafViewManager::selectView(mafCore::mafObjectBase *view) {
    REQUIRE(view != NULL);
    mafView *v = dynamic_cast<mafResources::mafView *>(view);
    if(v != NULL) {
        // A new mafView has been selected; update the m_ActiveResource variable.
        if(m_SelectedView != NULL) {
            m_SelectedView->select(false);
        }
        m_SelectedView = v;
        m_SelectedView->select(true);
    }
}

void mafViewManager::vmeShow(mafCore::mafObjectBase *vme, bool show) {
    mafVME *vme_resource = dynamic_cast<mafResources::mafVME *>(vme);
    if(vme_resource != NULL) {
        if(m_SelectedView) {
            m_SelectedView->showVME(vme_resource, show);
        }
    }
}

mafObjectBase *mafViewManager::createView(mafString view_type) {
    REQUIRE(view_type.length() > 0);

    mafObjectBase *obj = mafNEWFromString(view_type);
    mafView *v = dynamic_cast<mafResources::mafView *>(obj);
    if(v != NULL) {
        addViewToCreatedList(v);
        m_SelectedView = v;
        return v;
    } else {
        mafDEL(obj);
    }
    return NULL;
}

void mafViewManager::addViewToCreatedList(mafView *v) {
    if(v) {
        // Check the presence of the view in the list
        bool view_is_present = m_CreatedViewList.contains(v);
        if(!view_is_present) {
            // Connect the manager to the view destryed signal
            connect(v, SIGNAL(destroyed()), this, SLOT(viewDestroyed()));
            // add the new created view to the list.
            m_CreatedViewList.append(v);
            v->create();
        }
    }
}

void mafViewManager::destroyView(mafCore::mafObjectBase *view) {
    mafView *v = dynamic_cast<mafResources::mafView *>(view);
    if(v) {
        // Check if the view is present in the list
        bool view_is_present = m_CreatedViewList.contains(v);
        if(view_is_present) {
            removeView(v);
            // Destroy the view.
            mafDEL(v);
        }
    }
}

void mafViewManager::destroyAllViews() {
    mafResourceList viewList = m_CreatedViewList;
    foreach(mafResource *v, viewList) {
        destroyView(v);
    }
    viewList.clear();
}

void mafViewManager::viewDestroyed() {
    mafView *v = (mafView *)QObject::sender();
    removeView(v);
}

void mafViewManager::removeView(mafView *view) {
    // Disconnect the manager from the view
    disconnect(view, SIGNAL(destroyed()),this, SLOT(viewDestroyed()));
    // get the index of the view.
    int idx = m_CreatedViewList.indexOf(view, 0);
    // Remove the view from the list.
    if(m_CreatedViewList.removeOne(view)) {
        if(idx > 0) {
            m_SelectedView = (mafView *)m_CreatedViewList.at(idx - 1);
        } else {
            m_SelectedView = NULL;
        }
    }
}
