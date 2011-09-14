/*
 *  mafView.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 30/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
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

mafView::mafView(const QString code_location) : mafResource(code_location), m_RenderWidget(NULL), m_Scenegraph(NULL), m_VisualPipeHash(NULL), m_SelectedNode(NULL),m_PipeVisualSelection(NULL), m_VisibleObjects(0) {
    m_SceneNodeList.clear();

    // Callbacks related to the VME creation
    mafRegisterLocalCallback("maf.local.resources.vme.add", this, "vmeAdd(mafCore::mafObjectBase *)")
    // Callback related to the VME selection
    mafRegisterLocalCallback("maf.local.resources.vme.select", this, "vmeSelect(mafCore::mafObjectBase *)")
}


mafView::~mafView() {   
    clearScene();
    // Q_EMIT signal to detach visual pipe gui
    Q_EMIT pipeVisualSelectedSignal(NULL);
}


void mafView::clearScene() {
    mafDEL(m_Scenegraph);
    m_SelectedNode = NULL;
    
    m_SceneNodeList.clear();
}

void mafView::create() {
}


mafSceneNode *mafView::createSceneNode(mafVME *vme) {
    return new mafSceneNode(vme, m_RenderWidget, "", mafCodeLocation);
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
        m_SceneNodeList.push_back(node);
        
        if(m_SelectedNode == NULL) {
            vmeSelect(node);
        }
        
        mafDEL(node);
    }
}

void mafView::vmeSelect(mafObjectBase *node) {
    mafSceneNode *node_to_select = qobject_cast<mafResources::mafSceneNode *>(node);
    if(node_to_select == NULL) {
        mafVME *vme_to_select = qobject_cast<mafResources::mafVME *>(node);
        node_to_select = sceneNodeFromVme(vme_to_select);
    }
    if(NULL == node_to_select) {
        qWarning("%s", mafTr("Trying to select an object that is not present in tree.").toAscii().data());
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
        m_SceneNodeList.removeOne(node);
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

    if (vp == "") {
        //if originally in visual pipe hash, is not present that binding data-pipe visual,
        // request to the PluginManager possible visual pipe accepting vme object.
        mafPluggedObjectInformationList *vpsHash = mafPluginManager::instance()->queryPluggedObjects("mafResources::mafPipeVisual");
        
        qWarning("%s", mafTr("Visual pipe not found for '%1' of data!").arg(vp).toAscii().data());
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

mafSceneNode *mafView::sceneNodeFromVme(mafObjectBase *vme) {
    QListIterator<mafSceneNode *> list(m_SceneNodeList);
    while (list.hasNext()) {
        mafSceneNode *sn = list.next();
        if (sn->vme()->isEqual(vme)) {
            return sn;
        }
     }
     return NULL;
}

void mafView::select(bool select) {
    m_Selected = select;
    if (select) {
        notityVisualPipeSelected();
    }
}

void mafView::updateView() {
}

