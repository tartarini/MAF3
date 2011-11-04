/*
 *  mafViewManager.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
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
    m_VisualizationBindHash.clear();
    destroyAllViews();
}

mafViewManager::mafViewManager(const QString code_location) : mafObjectBase(code_location), m_SelectedView(NULL) {
    initializeConnections();
}

mafViewManager::~mafViewManager() {
    destroyAllViews();
    
    // Unregister callbacks...
    mafUnregisterLocalCallback("maf.local.resources.view.create", this, "createView(QString, QString)")    
    mafUnregisterLocalCallback("maf.local.resources.view.destroy", this, "destroyView(mafCore::mafObjectBase *)")
    mafUnregisterLocalCallback("maf.local.resources.view.select", this, "selectView(mafCore::mafObjectBase *)")
    mafUnregisterLocalCallback("maf.local.resources.view.selected", this, "selectedView()")
    mafUnregisterLocalCallback("maf.local.resources.view.sceneNodeShow", this, "sceneNodeShow(mafCore::mafObjectBase *, bool)")
    mafUnregisterLocalCallback("maf.local.logic.status.viewmanager.store", this, "createMemento()")
    mafUnregisterLocalCallback("maf.local.logic.status.viewmanager.restore", this, "setMemento(mafCore::mafMemento *, bool)")
    mafUnregisterLocalCallback("maf.local.resources.view.clearViews", this, "clearViews()")
    mafUnregisterLocalCallback("maf.local.resources.view.fillViews", this, "fillViews()")
    mafUnregisterLocalCallback("maf.local.resources.view.customizeVisualization", this, "customPipeVisualForVMEInView(QString, QString, QString)")
    mafUnregisterLocalCallback("maf.local.resources.view.sceneNodeReparent", this, "sceneNodeReparent(mafCore::mafObjectBase *, mafCore::mafObjectBase *)")

    
    // Unregister signals...
    mafUnregisterLocalSignal("maf.local.resources.view.create", this, "createViewSignal(QString, QString)")
    mafUnregisterLocalSignal("maf.local.resources.view.created", this, "viewCreatedSignal(mafCore::mafObjectBase *)")
    mafUnregisterLocalSignal("maf.local.resources.view.destroy", this, "destroyViewSignal(mafCore::mafObjectBase *)")
    mafUnregisterLocalSignal("maf.local.resources.view.select", this, "selectViewSignal(mafCore::mafObjectBase *)")
    mafUnregisterLocalSignal("maf.local.resources.view.selected", this, "selectedViewSignal(mafCore::mafObjectBase *)")
    mafUnregisterLocalSignal("maf.local.resources.view.sceneNodeReparent", this, "sceneNodeReparentSignal(mafCore::mafObjectBase *, mafCore::mafObjectBase *)")
    mafUnregisterLocalSignal("maf.local.resources.view.sceneNodeShow", this, "sceneNodeShowSignal(mafCore::mafObjectBase *, bool)")
    mafUnregisterLocalSignal("maf.local.resources.view.noneViews", this, "noneViewsSignal()")
    mafUnregisterLocalSignal("maf.local.resources.view.clearViews", this, "clearViewsSignal()")
    mafUnregisterLocalSignal("maf.local.resources.view.fillViews", this, "fillViewsSignal()")
    mafUnregisterLocalSignal("maf.local.resources.view.customizeVisualization", this, "customPipeVisualForVMEInViewSignal(QString, QString, QString)")

    
    // Remove IDs...
    mafIdProvider *provider = mafIdProvider::instance();
    provider->removeId("maf.local.resources.view.create");
    provider->removeId("maf.local.resources.view.created");
    provider->removeId("maf.local.resources.view.destroy");
    provider->removeId("maf.local.resources.view.select");
    provider->removeId("maf.local.resources.view.selected");
    provider->removeId("maf.local.resources.view.sceneNodeReparent");
    provider->removeId("maf.local.resources.view.sceneNodeShow");
    provider->removeId("maf.local.resources.view.noneViews");
    provider->removeId("maf.local.resources.view.clearViews");
    provider->removeId("maf.local.resources.view.fillViews");
    provider->removeId("maf.local.resources.view.customizeVisualization");
}

mafMemento *mafViewManager::createMemento() const {
    return new mafMementoViewManager(this, &m_CreatedViewList, mafCodeLocation);
}

void mafViewManager::setMemento(mafMemento *memento, bool deep_memento) {
    Q_UNUSED(deep_memento);

    QString viewType;
    QString viewName;
    mafMementoPropertyList *list = memento->mementoPropertyList();
    mafMementoPropertyItem item;
    Q_FOREACH(item, *list) {
        if(item.m_Name == "ViewType") {
            viewType = item.m_Value.toString();
            /*createView(viewType);*/
        }
        if(item.m_Name == "ViewName") {
            viewName = item.m_Value.toString();
            createView(viewType, viewName);
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
    provider->createNewId("maf.local.resources.view.clearViews");
    provider->createNewId("maf.local.resources.view.fillViews");
    provider->createNewId("maf.local.resources.view.customizeVisualization");
    
    // Register API signals.
    mafRegisterLocalSignal("maf.local.resources.view.create", this, "createViewSignal(QString, QString)")
    mafRegisterLocalSignal("maf.local.resources.view.created", this, "viewCreatedSignal(mafCore::mafObjectBase *)")
    mafRegisterLocalSignal("maf.local.resources.view.destroy", this, "destroyViewSignal(mafCore::mafObjectBase *)")
    mafRegisterLocalSignal("maf.local.resources.view.select", this, "selectViewSignal(mafCore::mafObjectBase *)")
    mafRegisterLocalSignal("maf.local.resources.view.selected", this, "selectedViewSignal()")
    mafRegisterLocalSignal("maf.local.resources.view.sceneNodeReparent", this, "sceneNodeReparentSignal(mafCore::mafObjectBase *, mafCore::mafObjectBase *)")
    mafRegisterLocalSignal("maf.local.resources.view.sceneNodeShow", this, "sceneNodeShowSignal(mafCore::mafObjectBase *, bool)")
    mafRegisterLocalSignal("maf.local.resources.view.noneViews", this, "noneViewsSignal()")
    mafRegisterLocalSignal("maf.local.resources.view.clearViews", this, "clearViewsSignal()")
    mafRegisterLocalSignal("maf.local.resources.view.fillViews", this, "fillViewsSignal()")
    mafRegisterLocalSignal("maf.local.resources.view.customizeVisualization", this, "customPipeVisualForVMEInViewSignal(QString, QString, QString)")

    // Register private callbacks to the instance of the manager..
    mafRegisterLocalCallback("maf.local.resources.view.create", this, "createView(QString, QString)")
    mafRegisterLocalCallback("maf.local.resources.view.destroy", this, "destroyView(mafCore::mafObjectBase *)")
    mafRegisterLocalCallback("maf.local.resources.view.select", this, "selectView(mafCore::mafObjectBase *)")
    mafRegisterLocalCallback("maf.local.resources.view.selected", this, "selectedView()")
    mafRegisterLocalCallback("maf.local.resources.view.sceneNodeShow", this, "sceneNodeShow(mafCore::mafObjectBase *, bool)")
    mafRegisterLocalCallback("maf.local.resources.view.clearViews", this, "clearViews()")
    mafRegisterLocalCallback("maf.local.resources.view.fillViews", this, "fillViews()")
    mafRegisterLocalCallback("maf.local.resources.view.customizeVisualization", this, "customPipeVisualForVMEInView(QString, QString, QString)")
    
    // Register callback to allows settings serialization.
    mafRegisterLocalCallback("maf.local.logic.status.viewmanager.store", this, "createMemento()")
    mafRegisterLocalCallback("maf.local.logic.status.viewmanager.restore", this, "setMemento(mafCore::mafMemento *, bool)")

    //CallBack related to the Scene node reparent
    mafRegisterLocalCallback("maf.local.resources.view.sceneNodeReparent", this, "sceneNodeReparent(mafCore::mafObjectBase *, mafCore::mafObjectBase *)")
}

void mafViewManager::customPipeVisualForVMEInView(QString view_name, QString data_type, QString pipe_type) {
    QHash<QString, QString> *hash;
    if (!m_VisualizationBindHash.contains(view_name)) {
        hash = new QHash<QString, QString>;
        m_VisualizationBindHash.insert(view_name, hash);
    }
    hash = m_VisualizationBindHash.value(view_name);
    hash->insert(data_type, pipe_type);
}

void mafViewManager::selectView(mafCore::mafObjectBase *view) {
    REQUIRE(view != NULL);
    mafView *v = qobject_cast<mafResources::mafView *>(view);
    if(m_SelectedView == view) {
        return;
    }
    if(v != NULL) {
        // A new mafView has been selected; update the m_ActiveResource variable.
        if(m_SelectedView != NULL) {
            m_SelectedView->select(false);
        }
        m_SelectedView = v;
        //View must know if it is the selected one
        //to inform GUIManager about visual pipe in use.
        m_SelectedView->select(true);
    }
}

void mafViewManager::sceneNodeShow(mafCore::mafObjectBase *node, bool show) {
    mafSceneNode *node_to_show = NULL;
    node_to_show = qobject_cast<mafResources::mafSceneNode *>(node);

    if(node_to_show == NULL && m_SelectedView) {
        //If node is not a sceneNode, check if its a VME and get the corresponding sceneNode
        mafVME *vme_to_show = qobject_cast<mafResources::mafVME *>(node);
        node_to_show = m_SelectedView->sceneNodeFromVme(vme_to_show);
    }
    
    if(node_to_show != NULL) {
        if(m_SelectedView) {
            m_SelectedView->showSceneNode(node_to_show, show);
            //m_SelectedPipeVisual = node_to_show->visualPipe();
        } else {
            QByteArray ba = mafTr("There is no view selected.").toAscii();
            qCritical(ba.constData());
        }
    }
}

void mafViewManager::createView(QString view_type, QString view_name) {
    REQUIRE(view_type.length() > 0);

    mafObjectBase *obj = mafNEWFromString(view_type);
    mafView *v = qobject_cast<mafResources::mafView *>(obj);
    if(v != NULL) {
        v->setObjectName(view_name);
        if (m_VisualizationBindHash.contains(view_name)) {
            v->plugVisualPipeBindingHash(m_VisualizationBindHash.value(view_name));
        }
        addViewToCreatedList(v);
        selectView(obj);
    } else {
        mafDEL(obj);
    }
}


mafObjectBase *mafViewManager::selectedView() {
    return m_SelectedView;
}

void mafViewManager::sceneNodeReparent(mafCore::mafObjectBase *vme, mafCore::mafObjectBase *vmeParent) {
    int i = 0;
    //Get VME inside sceneNode, in order to get the sceneNodes of each view
    mafSceneNode* sceneNode = qobject_cast<mafSceneNode*>(vme);
    mafSceneNode* sceneNodeParent = qobject_cast<mafSceneNode*>(vmeParent);

    for (; i < m_CreatedViewList.count(); i++) {
        mafView *view = (mafView *)m_CreatedViewList.at(i);
        mafCore::mafHierarchy *sceneGraph = view->hierarchy().value<mafCore::mafHierarchyPointer>();
        if (sceneGraph != NULL) {
            //get sceneNodes corresponding to VMEs from each view
            mafSceneNode* sn = view->sceneNodeFromVme(sceneNode->vme());
            mafSceneNode* snParent = view->sceneNodeFromVme(sceneNodeParent->vme());
            sceneGraph->reparentHierarchyNode(sn, snParent);
            sn->setParentNode(snParent);
        }
    }
}

void mafViewManager::addViewToCreatedList(mafView *v) {
    if(v) {
        // Check the presence of the view in the list
        bool view_is_present = m_CreatedViewList.contains(v);
        if(!view_is_present) {
            // Connect the manager to the view destroyed signal
            connect(v, SIGNAL(destroyed()), this, SLOT(viewDestroyed()));
            // add the new created view to the list.
            m_CreatedViewList.append(v);
            v->initialize();

            // Notify the view creation.
            mafEventArgumentsList argList;
            argList.append(mafEventArgument(mafCore::mafObjectBase*, v));
            mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.created", mafEventTypeLocal, &argList);
        }
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
    Q_FOREACH(mafResource *v, viewList) {
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
        if(m_CreatedViewList.count() > 0) {
            mafObjectBase *obj = m_CreatedViewList.first();
            selectView(obj);
        } else {
            m_SelectedView = NULL;
            // View list empty
            mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.noneViews");
        }
    }
}

void mafViewManager::clearView(mafCore::mafObjectBase *view) {
    mafView *v = qobject_cast<mafResources::mafView *>(view);
    if(v) {
        // Check if the view is present in the list
        bool view_is_present = m_CreatedViewList.contains(v);
        if(view_is_present) {
            v->clearScene();
        }
    }
}

void mafViewManager::clearViews() {
    mafResourceList viewList = m_CreatedViewList;
    Q_FOREACH(mafResource *v, viewList) {
        clearView(v);
    }
}

void mafViewManager::fillViews() {
    mafResourceList viewList = m_CreatedViewList;
    mafCore::mafHierarchyPointer hierarchy;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafHierarchyPointer, hierarchy);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.request", mafEventTypeLocal, NULL, &ret_val);
    Q_FOREACH(mafResource *v, viewList) {
        ((mafView *)v)->fillSceneGraph(hierarchy);
    }
}
