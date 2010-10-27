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

mafView::mafView(const mafString code_location) : mafResource(code_location), m_RenWindow(NULL), m_Scenegraph(NULL)  {
    m_CustomVisualPipeMap.clear();
    m_Selected = false;
}

mafView::~mafView() {
    if(m_Scenegraph != NULL) {
        m_Scenegraph->clear();
    }
    mafDEL(m_Scenegraph);
    m_CustomVisualPipeMap.clear();
}

void mafView::create() {
    m_Scenegraph = mafNEW(mafCore::mafHierarchy);
}

void mafView::addVME(mafVME *vme) {
    if(m_Scenegraph != NULL) {
//        mafSceneNode *node = m_Scenegraph->sceneNodeByVmeHash(vme->objectHash());
//        if (node == NULL) {
            mafSceneNode *node = new mafSceneNode(vme, NULL, mafCodeLocation);
            m_Scenegraph->addHierarchyNode(node);
//        }
    }
}

void mafView::removeVME(mafVME *vme) {
    if(m_Scenegraph != NULL) {
        mafVisitorFindSceneNodeByVMEHash *v = new mafVisitorFindSceneNodeByVMEHash(vme->objectHash(), mafCodeLocation);
        mafObjectRegistry::instance()->findObjectsThreaded(v);
        mafSceneNode *node = v->sceneNode();
        if (node != NULL) {
            m_Scenegraph->removeHierarchyNode(node);
        }
        mafDEL(v);
    }
}

void mafView::selectVME(mafVME *vme, bool select) {
    Q_UNUSED(vme);
    Q_UNUSED(select);
}

void mafView::showVME(mafVME *vme, bool show, const mafString visualPipeType) {
    mafString vp(visualPipeType);
    if (vp == "") {
        // Find custom visual pipe for this kind of data
        mafString dataType;
        mafDataSet *data = vme->outputData();
        if  (data != NULL) {
            dataType = data->dataValue()->externalDataType();
        }
        vp = m_CustomVisualPipeMap.value(dataType);
        if (vp == "") {
           mafMsgDebug("%s", mafTr("Visual pipe not found for '%1' of data!").arg(vp).toAscii().data());
           return;
        }
    }
    if(m_Scenegraph != NULL) {
        mafVisitorFindSceneNodeByVMEHash *v = new mafVisitorFindSceneNodeByVMEHash(vme->objectHash(), mafCodeLocation);
        mafObjectRegistry::instance()->findObjectsThreaded(v);
        mafSceneNode *node = v->sceneNode();
        mafDEL(v);
        if (node != NULL && show) {
            node->setVisualPipe(vp);
            node->visualPipe()->setInput(vme);
            node->visualPipe()->createPipe();
            node->visualPipe()->updatePipe();
        }
    }
}

void mafView::plugVisualPipe(mafString dataType, mafString visualPipeType) {
    if (!dataType.isEmpty() && !visualPipeType.isEmpty()) {
        m_CustomVisualPipeMap.insert(dataType, visualPipeType);
    }
}
