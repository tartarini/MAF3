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
#include "mafSceneNode.h"

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

mafViewManager::mafViewManager(const QString code_location) : mafObjectBase(code_location), m_SelectedView(NULL) {
    initializeConnections();
}

mafViewManager::~mafViewManager() {
    destroyAllViews();
}

mafMemento *mafViewManager::createMemento() const {
    return new mafMementoViewManager(this, &m_CreatedViewList, mafCodeLocation);
}

void mafViewManager::setMemento(mafMemento *memento, bool deep_memento) {
    Q_UNUSED(deep_memento);

    QString viewType;
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
    provider->createNewId("maf.local.resources.view.created");
    provider->createNewId("maf.local.resources.view.destroy");
    provider->createNewId("maf.local.resources.view.select");
    provider->createNewId("maf.local.resources.view.selected");
    provider->createNewId("maf.local.resources.view.sceneNodeReparent");
    provider->createNewId("maf.local.resources.view.sceneNodeShow");
    provider->createNewId("maf.local.resources.view.noneViews");

    // Register API signals.
    mafRegisterLocalSignal("maf.local.resources.view.create", this, "createViewSignal(QString)")
    mafRegisterLocalSignal("maf.local.resources.view.created", this, "viewCreatedSignal(mafCore::mafObjectBase *)")
    mafRegisterLocalSignal("maf.local.resources.view.destroy", this, "destroyViewSignal(mafCore::mafObjectBase *)")
    mafRegisterLocalSignal("maf.local.resources.view.select", this, "selectViewSignal(mafCore::mafObjectBase *)")
    mafRegisterLocalSignal("maf.local.resources.view.selected", this, "selectedViewSignal(mafCore::mafObjectBase *)")
    mafRegisterLocalSignal("maf.local.resources.view.sceneNodeReparent", this, "sceneNodeReparentSignal(mafCore::mafObjectBase *, mafCore::mafObjectBase *)")
    mafRegisterLocalSignal("maf.local.resources.view.sceneNodeShow", this, "sceneNodeShowSignal(mafCore::mafObjectBase *, bool)")
    mafRegisterLocalSignal("maf.local.resources.view.noneViews", this, "noneViewsSignal()")

    // Register private callbacks to the instance of the manager..
    mafRegisterLocalCallback("maf.local.resources.view.create", this, "createView(QString)")
    mafRegisterLocalCallback("maf.local.resources.view.destroy", this, "destroyView(mafCore::mafObjectBase *)")
    mafRegisterLocalCallback("maf.local.resources.view.select", this, "selectView(mafCore::mafObjectBase *)")
    mafRegisterLocalCallback("maf.local.resources.view.sceneNodeShow", this, "sceneNodeShow(mafCore::mafObjectBase *, bool)")

    // Register callback to allows settings serialization.
    mafRegisterLocalCallback("maf.local.logic.status.viewmanager.store", this, "createMemento()")
    mafRegisterLocalCallback("maf.local.logic.status.viewmanager.restore", this, "setMemento(mafCore::mafMemento *, bool)")
}

void mafViewManager::selectView(mafCore::mafObjectBase *view) {
    REQUIRE(view != NULL);
    mafView *v = qobject_cast<mafResources::mafView *>(view);
    if(v != NULL) {
        // A new mafView has been selected; update the m_ActiveResource variable.
        if(m_SelectedView != NULL) {
            m_SelectedView->select(false);
        }
        m_SelectedView = v;
        m_SelectedView->select(true); // ?!?

        // Notify the view selection.
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafObjectBase*, m_SelectedView));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.selected", mafEventTypeLocal, &argList);
    }
}

void mafViewManager::sceneNodeShow(mafCore::mafObjectBase *node, bool show) {
    mafSceneNode *node_to_show = NULL;
    QString nodeName = "mafResources::mafVME";
    if (nodeName.compare(node->metaObject()->className()) == 0) {
      node_to_show = m_SelectedView->sceneNodeFromVme(node);
    } else {
      node_to_show = qobject_cast<mafResources::mafSceneNode *>(node);
    }
    if(node_to_show != NULL) {
        if(m_SelectedView) {
            m_SelectedView->showSceneNode(node_to_show, show, "mafPluginVTK::mafPipeVisualVTKSurface");
        }
    }
}

void mafViewManager::createView(QString view_type) {
    REQUIRE(view_type.length() > 0);

    mafObjectBase *obj = mafNEWFromString(view_type);
    mafView *v = qobject_cast<mafResources::mafView *>(obj);
    if(v != NULL) {
        addViewToCreatedList(v);
        selectView(obj);
    } else {
        mafDEL(obj);
    }
}

void mafViewManager::addViewToCreatedList(mafView *v) {
    if(v) {
        // Check the presence of the view in the list
        bool view_is_present = m_CreatedViewList.contains(v);
        if(!view_is_present) {
            // TODO: add to the new view all the created VME wrapped into the mafSceneNode each one.
            // Connect the manager to the view destryed signal
            connect(v, SIGNAL(destroyed()), this, SLOT(viewDestroyed()));
            // add the new created view to the list.
            m_CreatedViewList.append(v);
            v->create();

            // create sceneNode hierarchy equal to vme hierarchy
            mafCore::mafHierarchyPointer hierarchy;
            QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafHierarchyPointer, hierarchy);
            mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.request", mafEventTypeLocal, NULL, &ret_val);
            mafTree<QObject *>::iterator temp_iterator = hierarchy->iterator();

            //Create root node
            v->selectSceneNode(NULL, false);
            hierarchy->moveTreeIteratorToRootNode();
            QObject* rootNode = hierarchy->currentData();
            v->vmeAdd(qobject_cast<mafCore::mafObjectBase *>(rootNode));
            this->fillSceneGraph(v, hierarchy);

            hierarchy->setIterator(temp_iterator);
            QObject* selectedVME = hierarchy->currentData();
            mafSceneNode *selectedNode = v->sceneNodeFromVme(qobject_cast<mafCore::mafObjectBase *>(selectedVME));
            v->selectSceneNode(selectedNode, selectedNode->property("canVisualize").toBool());
            // Notify the view creation.
            mafEventArgumentsList argList;
            argList.append(mafEventArgument(mafCore::mafObjectBase*, v));
            mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.created", mafEventTypeLocal, &argList);
        }
    }
}

void mafViewManager::fillSceneGraph(mafView *v, mafCore::mafHierarchy *hierarchy) {
  int i = 0, size = hierarchy->currentNumberOfChildren();
  for(i; i < size; i++) {
    hierarchy->moveTreeIteratorToNthChild(i);
    QObject *vme = hierarchy->currentData();
    hierarchy->moveTreeIteratorToParent();
    QObject *vmeParent = hierarchy->currentData();
    mafSceneNode *parentNode = v->sceneNodeFromVme(qobject_cast<mafCore::mafObjectBase *>(vmeParent));
    v->selectSceneNode(parentNode, parentNode->property("visibility").toBool());
    v->vmeAdd(qobject_cast<mafCore::mafObjectBase *>(vme));
    hierarchy->moveTreeIteratorToNthChild(i);
    fillSceneGraph(v, hierarchy);
    hierarchy->moveTreeIteratorToParent();
  }
}

void mafViewManager::destroyView(mafCore::mafObjectBase *view) {
    mafView *v = qobject_cast<mafResources::mafView *>(view);
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
            mafObjectBase *obj = m_CreatedViewList.at(idx - 1);
            selectView(obj);
        } else {
            m_SelectedView = NULL;
            // View list empty
            mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.noneViews");
        }
    }
}
