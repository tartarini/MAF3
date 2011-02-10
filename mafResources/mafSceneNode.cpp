/*
 *  mafSceneNode.cpp
 *  mafResources
 *
 *  Created by Daniele Giunchi on 24/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafSceneNode.h"
#include "mafVisualPipe.h"
#include "mafVME.h"

using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

mafSceneNode::mafSceneNode(const mafString code_location) : mafObject(code_location), m_VisualPipe(NULL) {
}

mafSceneNode::mafSceneNode(mafVME *vme, mafVisualPipe *visual_pipe, const mafString code_location) : mafObject(code_location), m_VME(vme), m_VisualPipe(visual_pipe) {
    connect(vme, SIGNAL(destroyed()), this, SIGNAL(destroyNode()));
    connect(vme, SIGNAL(detatched()), this, SIGNAL(destroyNode()));
}

mafSceneNode::~mafSceneNode() {
    mafDEL(this->m_VisualPipe);
}

void mafSceneNode::setSelected(bool sel) {
    if(m_Selected != sel) {
        m_Selected = sel;
        if(m_Selected) {
            // notify the VME selection.
            mafEventArgumentsList argList;
            argList.append(mafEventArgument(mafCore::mafObjectBase *, m_VME));
            mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.select", mafEventTypeLocal, &argList);
        }
    }
}

void mafSceneNode::visualPipeDestroyed() {
    this->m_VisualPipe = NULL;
}

void mafSceneNode::setVisualPipe(mafString visualPipeType) {
    if(m_VisualPipe != NULL) {
        mafString currentVisualPipeType = this->m_VisualPipe->metaObject()->className();
        if (visualPipeType.compare(currentVisualPipeType) == 0) {
            return;
        }
    }

    mafDEL(this->m_VisualPipe);
    this->m_VisualPipe = (mafVisualPipe *)mafNEWFromString(visualPipeType);
    connect(m_VisualPipe, SIGNAL(destroyed()), this, SLOT(visualPipeDestroyed()));
}

