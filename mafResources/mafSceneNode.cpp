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

using namespace mafCore;
using namespace mafResources;

mafSceneNode::mafSceneNode(const mafString code_location) : mafObject(code_location), m_VisualPipe(NULL) {
}

mafSceneNode::mafSceneNode(mafVME *vme, mafVisualPipe *visual_pipe, const mafString code_location) : mafObject(code_location), m_VME(vme), m_VisualPipe(visual_pipe) {
}

mafSceneNode::~mafSceneNode() {
    mafDEL(this->m_VisualPipe);
}

void mafSceneNode::setVisualPipe(mafString visualPipeType) {
    if (this->m_VisualPipe == NULL) {
        this->m_VisualPipe = (mafVisualPipe *)mafNEWFromString(visualPipeType);
        return;
    }
    mafString visualPipeName = this->m_VisualPipe->metaObject()->className();
    if (visualPipeType.compare(visualPipeName) != 0) {
        mafDEL(this->m_VisualPipe);
        this->m_VisualPipe = (mafVisualPipe *)mafNEWFromString(visualPipeType);
    }
}

