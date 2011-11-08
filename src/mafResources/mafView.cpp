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

using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

mafView::mafView(const QString code_location) : mafResource(code_location), m_RenderWidget(NULL), m_Scenegraph(NULL), m_VisualPipeHash(NULL), m_SelectedNode(NULL),m_PipeVisualSelection(NULL), m_VisibleObjects(0), m_LayoutConfigurationFile("") {
    m_SceneNodeHash.clear();

    // Callbacks related to the VME creation
    mafRegisterLocalCallback("maf.local.resources.vme.add", this, "vmeAdd(mafCore::mafObjectBase *)")
    // Callback related to the VME selection
    mafRegisterLocalCallback("maf.local.resources.vme.select", this, "vmeSelect(mafCore::mafObjectBase *)")
}


mafView::~mafView() {
    clearScene();
    // Q_EMIT signal to detach visual pipe GUI
    Q_EMIT pipeVisualSelectedSignal(NULL);
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
    mafDEL(m_Scenegraph);
    m_SelectedNode = NULL;
    
    m_SceneNodeHash.clear();
    Q_EMIT pipeVisualSelectedSignal(NULL);
    if(m_PipeVisualSelection) {
        m_PipeVisualSelection->setInput(NULL);
    }
}

mafSceneNode *mafView::createSceneNode(mafVME *vme) {
    return new mafSceneNode(vme, m_RenderWidget, "", mafCodeLocation);
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

    if(node && m_PipeVisualSelection) {
        if (node->property("visibility").toBool()) {
            //This code starts the loading of data, so 
            //it is called only if the node is visible.
            m_PipeVisualSelection->setInput(node->vme());
            m_PipeVisualSelection->updatePipe();
        }
        m_PipeVisualSelection->setVisibility(node->property("visibility").toBool());
        if (m_Selected) {
            notityVisualPipeSelected();
        }
    }
}

void mafView::notityVisualPipeSelected() {
    //Q_EMIT signal to inform about visual pipe of the current node.
    Q_EMIT pipeVisualSelectedSignal(m_SelectedNode->visualPipe());
}

void mafView::showSceneNode(mafSceneNode *node, bool show) {
    REQUIRE(node != NULL);

    if(node->vme() == NULL) {
        return;
    }

    // Find visual pipe for this kind of data
    QString vp;
    QString dataType;
    mafDataSet *data = node->vme()->outputData();
    if  (data != NULL) {
        dataType = data->externalDataType();
    }
    if (m_VisualPipeHash && m_VisualPipeHash->contains(dataType)) {
        vp = m_VisualPipeHash->value(dataType);
    }

    if (vp.isEmpty()) {
        //if originally in visual pipe hash, is not present that binding data-pipe visual,
        // request to the PluginManager possible visual pipe accepting vme object.
        mafPluggedObjectInformationList *vpsHash = mafPluginManager::instance()->queryPluggedObjects("mafResources::mafPipeVisual");
        QByteArray ba = mafTr("Visual pipe not found for '%1'!").arg(dataType).toAscii();
        qWarning("%s", ba.data());
        return;
    }

    if(m_Scenegraph != NULL) {
        if (show) {
            ++m_VisibleObjects;
            node->setVisualPipe(vp);
        } else {
            --m_VisibleObjects;
        }
        node->setVisibility(show);
        if(m_PipeVisualSelection) {
            m_PipeVisualSelection->setVisibility(show);
        }
    }
}


void mafView::plugVisualPipeBindingHash(QHash<QString , QString> *hash) {
    m_VisualPipeHash = hash;
}

QVariantHash mafView::visualPipeHash() const {
    QVariantHash h;
    QHash<QString, QString>::iterator iter = m_VisualPipeHash->begin();
    while (iter != m_VisualPipeHash->end()) {
        QVariant v(iter.value());
        h.insert(iter.key(), v);
        ++iter;
    }
    return h;
}

void mafView::setVisualPipeHash(const QVariantHash hash) {
    if (m_VisualPipeHash == NULL) {
        m_VisualPipeHash = new QHash<QString, QString>();
    }
    QVariantHash::const_iterator iter = hash.constBegin();
    while (iter != hash.constEnd()) {
        m_VisualPipeHash->insert(iter.key(), iter.value().toString());
        ++iter;
    }
}

mafSceneNode *mafView::sceneNodeFromVme(mafObjectBase *vme) {
    REQUIRE(vme);
    mafVME *asked_vme = qobject_cast<mafResources::mafVME *>(vme);
    return m_SceneNodeHash.value(asked_vme, NULL);
}

void mafView::select(bool select) {
    m_Selected = select;
    if (select) {
        notityVisualPipeSelected();
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
        sn->setProperty("iconFile", sn->vme()->property("iconFile"));
        ++iter;
    }
}
