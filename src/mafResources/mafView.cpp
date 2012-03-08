/*
 *  mafView.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/03/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafView.h"
#include "mafVME.h"
#include "mafVisitorFindSceneNodeByVMEHash.h"
#include "mafSceneNode.h"
#include "mafPipeVisual.h"
#include "mafPluginManager.h"
#include "mafPlugin.h"
#include "mafDataSet.h"
#include "mafDataSetCollection.h"
#include "mafVMEManager.h"
#include "mafToolHandler.h"

using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

mafView::mafView(const QString code_location) : mafResource(code_location), 
                                                m_RenderWidget(NULL), 
                                                m_Scenegraph(NULL), 
                                                m_VisualPipeHash(NULL), 
                                                m_SelectedNode(NULL),
                                                m_VisibleObjects(0), 
                                                m_SceneNodeType("mafResources::mafSceneNode"),
                                                m_ToolHandler(NULL),
                                                m_LayoutConfigurationFile("") {
    m_SceneNodeHash.clear();

    m_ToolHandler = mafNEW(mafResources::mafToolHandler);
    connect(m_ToolHandler, SIGNAL(destroyed()), this, SLOT(resetToolHandler()));

    // Callbacks related to the VME creation
    mafRegisterLocalCallback("maf.local.resources.vme.add", this, "vmeAdd(mafCore::mafObjectBase *)")
    // Callback related to the VME selection
    mafRegisterLocalCallback("maf.local.resources.vme.select", this, "vmeSelect(mafCore::mafObjectBase *)")
}

mafView::~mafView() {
    mafDEL(m_ToolHandler);
    clearScene();
}

void mafView::fillSceneGraph(mafCore::mafHierarchy *hierarchy) {
    int i = 0, size = hierarchy->currentNumberOfChildren();
    for(i; i < size; i++) {
        hierarchy->moveTreeIteratorToNthChild(i);
        QObject *vme = hierarchy->currentData();
        hierarchy->moveTreeIteratorToParent();
        QObject *vmeParent = hierarchy->currentData();
        mafSceneNode *parentNode = this->sceneNodeFromVme(qobject_cast<mafCore::mafObjectBase *>(vmeParent));
        this->selectSceneNode(parentNode, parentNode->property("visibility").toBool());
        this->vmeAdd(qobject_cast<mafCore::mafObjectBase *>(vme));
        hierarchy->moveTreeIteratorToNthChild(i);
        fillSceneGraph(hierarchy);
        hierarchy->moveTreeIteratorToParent();
    }

    this->updateSceneNodesInformation();
}

void mafView::clearScene() {
    if (m_ToolHandler) {
        m_ToolHandler->setActiveSceneNode(NULL);
    }
    mafDEL(m_Scenegraph);
    m_SelectedNode = NULL;
    
    m_SceneNodeHash.clear();
    Q_EMIT pipeVisualSignal(NULL);
}

mafSceneNode *mafView::createSceneNode(mafVME *vme) {
    mafObjectBase *obj = mafNEWFromString(m_SceneNodeType);
    mafSceneNode *n = qobject_cast<mafResources::mafSceneNode *>(obj);
    if (n == NULL) {
        qCritical() << mafTr("Problem creating node of type %1").arg(m_SceneNodeType);
        return NULL;
    }
    n->m_VME = vme;
    n->m_GraphicObject = m_RenderWidget;
    n->initialize();
    return n;
}

void mafView::setupSceneGraph() {
    // create sceneNode hierarchy equal to vme hierarchy
    mafCore::mafHierarchyPointer hierarchy;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafHierarchyPointer, hierarchy);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.request", mafEventTypeLocal, NULL, &ret_val);

    //Create root SceneNode
    this->selectSceneNode(NULL, false);
    hierarchy->moveTreeIteratorToRootNode();
    QObject* rootNode = hierarchy->currentData();
    this->vmeAdd(qobject_cast<mafCore::mafObjectBase *>(rootNode));
    //Fill the new scene graph, with all the created VME wrapped into the mafSceneNode each one.
    this->fillSceneGraph(hierarchy);

    //Set VME hierarchy iterator to the original position.
    mafCore::mafObjectBase *selectedVME;
    ret_val = mafEventReturnArgument(mafCore::mafObjectBase *, selectedVME);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.selected", mafEventTypeLocal, NULL, &ret_val);
    mafSceneNode *selectedNode = this->sceneNodeFromVme(qobject_cast<mafCore::mafObjectBase *>(selectedVME));
    this->selectSceneNode(selectedNode, selectedNode->property("visualizationStatus").toUInt() == mafVisualizationStatusVisible);
}

void mafView::vmeAdd(mafCore::mafObjectBase *vme) {
    mafVME *vme_to_add = qobject_cast<mafResources::mafVME *>(vme);
    if(vme_to_add != NULL) {
        
        mafSceneNode *node = createSceneNode(vme_to_add);
        node->setObjectName(vme_to_add->objectName());
        connect(node, SIGNAL(destroyNode()), this, SLOT(sceneNodeDestroy()));

        mafDataSetCollection *datSetCollection = vme_to_add->dataSetCollection();
        if (datSetCollection)
        {
          connect(datSetCollection, SIGNAL(modifiedObject()), this, SLOT(updateView()));
        }

        if(m_Scenegraph == NULL) {
            m_Scenegraph = mafNEW(mafCore::mafHierarchy);
        }
        
        node->setParentNode(m_SelectedNode);
        m_Scenegraph->addHierarchyNode(node, m_SelectedNode);
        m_SceneNodeHash.insert(vme_to_add, node);
        
        if(m_SelectedNode == NULL) {
            vmeSelect(node);
        }
        
        mafDEL(node);
    }
}

void mafView::vmeSelect(mafObjectBase *node) {
    // Check if it is a mafSceneNode...
    mafSceneNode *node_to_select = qobject_cast<mafResources::mafSceneNode *>(node);
    if(node_to_select == NULL) {
        // Check if it is a mafVME...
        mafVME *vme_to_select = qobject_cast<mafResources::mafVME *>(node);
        node_to_select = sceneNodeFromVme(vme_to_select);
    }
    if(NULL == node_to_select) {
        // Something wrong happened, generate a warning...
        QByteArray ba = mafTr("Trying to select an object that is not present in tree.").toAscii();
        qWarning("%s", ba.data());
        return;
    }
    selectSceneNode(node_to_select, true);
}

void mafView::sceneNodeDestroy() {
    mafSceneNode *node = (mafSceneNode *)QObject::sender();
    removeSceneNode(node);
}

void mafView::removeSceneNode(mafSceneNode *node) {
    REQUIRE(node!= NULL);

    if (m_ToolHandler) {
        m_ToolHandler->setActiveSceneNode(NULL);
    }
    // Disconnect the view from the node
    disconnect(node, SIGNAL(destroyNode()),this, SLOT(sceneNodeDestroyed()));
    if(m_Scenegraph != NULL) {
        m_Scenegraph->removeHierarchyNode(node);
        m_SceneNodeHash.remove(node->vme());
    }
}

void mafView::selectSceneNode(mafSceneNode *node, bool select) {
    m_SelectedNode = node;
    Q_UNUSED(select);

    if(m_SelectedNode) {
        if (m_Selected) {
            notityVisualPipe();
        }
        if (m_ToolHandler) {
            m_ToolHandler->setActiveSceneNode(m_SelectedNode);
        }
    }
}

void mafView::notityVisualPipe() {
    //Q_EMIT signal to inform about visual pipe of the current node.
    mafCore::mafObjectBase *pipe = NULL;
    if(m_SelectedNode && m_SelectedNode->visualPipe()) {
        pipe = m_SelectedNode->visualPipe()->visibility() ? m_SelectedNode->visualPipe() : NULL;
    }
    Q_EMIT pipeVisualSignal(pipe);
}

void mafView::showSceneNode(mafSceneNode *node, bool show) {
    REQUIRE(node != NULL);

    if(node->vme() == NULL) {
        return;
    }

    // Find visual pipe for this kind of data
    QString visualPipeType("");
    QString dataType("");
    mafDataSet *data = node->vme()->outputData();
    if  (data != NULL) {
        dataType = data->externalDataType();
    }
    if (m_VisualPipeHash && m_VisualPipeHash->contains(dataType)) {
        visualPipeType = m_VisualPipeHash->value(dataType).toString();
    }

    if (visualPipeType.isEmpty()) {
        //if originally in visual pipe hash, is not present that binding data-pipe visual,
        // request to the PluginManager possible visual pipe accepting vme object.
        mafPluggedObjectInformationList *vpsHash = mafPluginManager::instance()->queryPluggedObjects("mafResources::mafPipeVisual");
        QByteArray ba = mafTr("Visual pipe not found for '%1'!").arg(dataType).toAscii();
        qWarning("%s", ba.data());
    }

    if(m_Scenegraph != NULL) {
        if (!visualPipeType.isEmpty()) {
            if (show) {
                ++m_VisibleObjects;
                node->setVisualPipe(visualPipeType);
            } else {
                --m_VisibleObjects;
            }
        }
        node->setVisibility(show);
    }
    if (node->visualPipe() && m_PipeParametersBindHash.contains(visualPipeType)) {
        node->visualPipe()->setParametersHash(m_PipeParametersBindHash.value(visualPipeType));
    }

    if (m_ToolHandler) {
        m_ToolHandler->setActiveSceneNode(node);
        m_ToolHandler->setVisibility(show);
    }
    
    notityVisualPipe();
    updateView();
}

mafSceneNode *mafView::sceneNodeFromVme(mafObjectBase *vme) {
    REQUIRE(vme);
    mafVME *asked_vme = qobject_cast<mafResources::mafVME *>(vme);
    return m_SceneNodeHash.value(asked_vme, NULL);
}

void mafView::select(bool select) {
    m_Selected = select;
    if (select) {
        notityVisualPipe();
    }
}

void mafView::updateView() {
}

void mafView::resetVisualization(double *bounds) {

}

void mafView::updateSceneNodesInformation() {
    QHash<mafVME *, mafSceneNode *>::iterator iter = m_SceneNodeHash.begin();
    while (iter != m_SceneNodeHash.end()) {
        mafSceneNode *sn = iter.value();
        
        /// @@TODO re-factor memento vme for handling Properties before adding vme. (can be a new signal for example load)
        sn->setObjectName(sn->vme()->objectName());
        sn->setProperty("iconType", sn->vme()->property("iconType"));
        ++iter;
    }
}
