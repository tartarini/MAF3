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
#include "mafVisualPipe.h"
#include "mafPluginManager.h"
#include "mafPlugin.h"
#include "mafDataSet.h"

using namespace mafCore;
using namespace mafResources;

mafView::mafView(const mafString code_location) : mafResource(code_location), m_RenderWidget(NULL), m_Scenegraph(NULL)  {
    m_VisualPipeHash.clear();
    m_Selected = false;

    // Callbacks related to the VME creation
    mafRegisterLocalCallback("maf.local.resources.vme.add", this, "vmeAdd(mafCore::mafObjectBase *)");
}

mafView::~mafView() {
    if(m_Scenegraph != NULL) {
        m_Scenegraph->clear();
    }
    mafDEL(m_Scenegraph);
}

void mafView::create() {
    m_Scenegraph = mafNEW(mafCore::mafHierarchy);
}

void mafView::vmeAdd(mafCore::mafObjectBase *vme) {
    mafVME *vme_to_add = dynamic_cast<mafResources::mafVME *>(vme);
    if(vme_to_add != NULL) {
        mafSceneNode *node = new mafSceneNode(vme_to_add, NULL, mafCodeLocation);
        connect(node, SIGNAL(destroyNode()), this, SLOT(sceneNodeDestroy()));
        if(m_Scenegraph != NULL) {
            m_Scenegraph->addHierarchyNode(node);
        }
    }
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
    }
}

void mafView::selectSceneNode(mafSceneNode *node, bool select) {
    Q_UNUSED(node);
    Q_UNUSED(select);
}

void mafView::showSceneNode(mafSceneNode *node, bool show, const mafString visualPipeType) {
    REQUIRE(node != NULL);

    if(node->vme() == NULL) {
        return;
    }

    mafString vp(visualPipeType);
    if (vp == "") {
        // Find visual pipe for this kind of data
        mafString dataType;
        mafDataSet *data = node->vme()->outputData();
        if  (data != NULL) {
            dataType = data->dataValue()->externalDataType();
        }
        vp = m_VisualPipeHash.value(dataType);
        if (vp == "") {
           mafMsgDebug("%s", mafTr("Visual pipe not found for '%1' of data!").arg(vp).toAscii().data());
           return;
        }
    }
    if(m_Scenegraph != NULL) {
        if (show) {
            node->setVisualPipe(vp);
            mafVisualPipe *pipe = node->visualPipe();
            if(pipe == NULL) {
                mafMsgWarning() << mafTr("No visual pipe type '") << vp << mafTr("'' registered!!");
                return;
            }
            pipe->setInput(node->vme());
            pipe->createPipe();
            pipe->updatePipe();
            // TODO: Connect the visivility property of the VME with the visibile
            // slot of the visual pipe to put in synch both the opbjects.
        } else {
            // TODO: Implement the case the show is false => destroy the
            // visualVipie?? Hide its actor ??
        }
    }
}

void mafView::plugVisualPipe(mafString dataType, mafString visualPipeType) {
    if (!dataType.isEmpty() && !visualPipeType.isEmpty()) {
        m_VisualPipeHash.insert(dataType, visualPipeType);
    }
}
