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

mafView::mafView(const QString code_location) : mafResource(code_location), m_RenderWidget(NULL), m_Scenegraph(NULL), m_SelectedNode(NULL)  {
    m_VisualPipeHash.clear();
    m_SceneNodeList.clear();

    // CallBack related to the Scene node reparent
    mafRegisterLocalCallback("maf.local.resources.view.sceneNodeReparent", this, "sceneNodeReparent(mafCore::mafObjectBase *, mafCore::mafObjectBase *)")


    // Callbacks related to the VME creation
    mafRegisterLocalCallback("maf.local.resources.vme.add", this, "vmeAdd(mafCore::mafObjectBase *)")
    // Callback related to the VME selection
    mafRegisterLocalCallback("maf.local.resources.vme.select", this, "vmeSelect(mafCore::mafObjectBase *)")
}


mafView::~mafView() {
    if(m_Scenegraph != NULL) {
        m_Scenegraph->clear();
    }
    mafDEL(m_Scenegraph);
    m_SelectedNode = NULL;
    m_SceneNodeList.clear();
}

void mafView::create() {
    m_Scenegraph = mafNEW(mafCore::mafHierarchy);
}

void mafView::sceneNodeReparent(mafCore::mafObjectBase *vme, mafCore::mafObjectBase *vmeParent) {
  if(m_Scenegraph != NULL) {
    m_Scenegraph->reparentHierarchyNode(vme, vmeParent);
  }
}

void mafView::vmeAdd(mafCore::mafObjectBase *vme) {
    mafVME *vme_to_add = qobject_cast<mafResources::mafVME *>(vme);
    if(vme_to_add != NULL) {
        mafSceneNode *node = new mafSceneNode(vme_to_add, "mafResources::mafPipeVisualBox", mafCodeLocation);
        node->setObjectName(vme_to_add->objectName());
        connect(node, SIGNAL(destroyNode()), this, SLOT(sceneNodeDestroy()));

        mafDataSetCollection *datSetCollection = vme_to_add->dataSetCollection();
        if (datSetCollection)
        {
          connect(datSetCollection, SIGNAL(modifiedObject()), this, SLOT(updateView()));
        }

        if(m_Scenegraph != NULL) {
            m_Scenegraph->addHierarchyNode(node, m_SelectedNode);
            m_SceneNodeList.push_back(node);
        }
        mafDEL(node);
    }
}

void mafView::vmeSelect(mafObjectBase *node) {
    mafSceneNode *node_to_select = qobject_cast<mafResources::mafSceneNode *>(node);
    if(NULL == node_to_select) {
        qWarning("%s", mafTr("Trying to select an object that not represent a mafSceneNode.").toAscii().data());
        return;
    }
    m_SelectedNode = node_to_select;//sceneNodeFromVme(vme);
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
}

void mafView::showSceneNode(mafSceneNode *node, bool show, const QString visualPipeType) {
    REQUIRE(node != NULL);

    if(node->vme() == NULL) {
        return;
    }

    QString vp(visualPipeType);
    if (vp == "") {
        // Find visual pipe for this kind of data
        QString dataType;
        mafDataSet *data = node->vme()->outputData();
        if  (data != NULL) {
            dataType = data->dataValue()->externalDataType();
        }
        //vp = m_VisualPipeHash.value(dataType);
        vp = node->visualPipeType();
        if (vp == "") {
           qDebug("%s", mafTr("Visual pipe not found for '%1' of data!").arg(vp).toAscii().data());
           return;
        }
    }
    if(m_Scenegraph != NULL) {
        if (show) {
            node->setVisualPipe(vp);
            if (node->visualPipe() != NULL) {
                node->visualPipe()->setGraphicObject(m_RenderWidget);
            }
        } 
        node->setVisibility(show);
    }
}


void mafView::plugVisualPipe(QString dataType, QString visualPipeType) {
    if (!dataType.isEmpty() && !visualPipeType.isEmpty()) {
        m_VisualPipeHash.insert(dataType, visualPipeType);
    }
}

mafSceneNode *mafView::sceneNodeFromVme(mafObjectBase *vme) {
    bool found = false;
    QListIterator<mafSceneNode *> list(m_SceneNodeList);
    while (list.hasNext()) {
        mafSceneNode *sn = list.next();
        if (sn->vme()->isEqual(vme)) {
            found = true;
            return sn;
        }
     }
    if (!found)
        return NULL;
}

void mafView::updateView() {
}

