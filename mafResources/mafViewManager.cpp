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
}

mafViewManager::mafViewManager(const mafString code_location) : mafObjectBase(code_location), m_SelectedView(NULL) {
    initializeConnections();
}

mafViewManager::~mafViewManager() {
    foreach(mafResource *v, m_CreatedViewList) {
        mafDEL(v);
    }
    m_CreatedViewList.clear();
}

void mafViewManager::initializeConnections() {
    // Create the IDs required to add a resource to the management system.
    mafIdProvider *provider = mafIdProvider::instance();
    provider->createNewId("RESOURCES_VIEW_CREATE");
    provider->createNewId("RESOURCES_VIEW_DESTROY");
    provider->createNewId("RESOURCES_VIEW_SELECT");
    provider->createNewId("RESOURCES_VIEW_SELECTED");
    provider->createNewId("RESOURCES_VME_SHOW");

    // Register API signals.
    mafRegisterLocalSignal("RESOURCES_VIEW_CREATE", this, "createViewSignal(mafString)");
    mafRegisterLocalSignal("RESOURCES_VIEW_DESTROY", this, "destroyViewSignal(mafCore::mafObjectBase *)");
    mafRegisterLocalSignal("RESOURCES_VIEW_SELECT", this, "selectViewSignal(mafCore::mafObjectBase *)");
    mafRegisterLocalSignal("RESOURCES_VIEW_SELECTED", this, "selectedViewSignal()");
    mafRegisterLocalSignal("RESOURCES_VME_SHOW", this, "vmeShowSignal(mafCore::mafObjectBase *, bool)");

    // Register private callbacks to the instance of the manager..
    mafRegisterLocalCallback("RESOURCES_VIEW_CREATE", this, "createView(mafString)");
    mafRegisterLocalCallback("RESOURCES_VIEW_DESTROY", this, "destroyView(mafCore::mafObjectBase *)");
    mafRegisterLocalCallback("RESOURCES_VIEW_SELECT", this, "selectView(mafCore::mafObjectBase *)");
    mafRegisterLocalCallback("RESOURCES_VIEW_SELECTED", this, "selectedView()");
    mafRegisterLocalCallback("RESOURCES_VME_SHOW", this, "vmeShow(mafCore::mafObjectBase *, bool)");
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
            // Destroy the view .
            mafDEL(v);
        }
    }
}

void mafViewManager::viewDestroyed() {
    mafView *v = (mafView *)QObject::sender();
    removeView(v);
}

void mafViewManager::removeView(mafView *view) {
    // Disconnect the manager from the view
    disconnect(view, SIGNAL(destroyed()),this, SLOT(viewDestroyed()));
    // Remove the view from the list.
    m_CreatedViewList.removeOne(view);
}
