/*
 *  mafVisitorFindSceneNodeByVMEHash.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 10/05/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVisitorFindSceneNodeByVMEHash.h"
#include "mafSceneNode.h"
#include "mafVME.h"

using namespace mafCore;
using namespace mafResources;

mafVisitorFindSceneNodeByVMEHash::mafVisitorFindSceneNodeByVMEHash(const QString code_location) : mafVisitorFindObjects(code_location), m_HashCode(""), m_SceneNode(NULL) {
}

mafVisitorFindSceneNodeByVMEHash::mafVisitorFindSceneNodeByVMEHash(const QString hash_code, const QString code_location) : mafVisitorFindObjects(code_location) , m_HashCode(hash_code), m_SceneNode(NULL) {
}

mafVisitorFindSceneNodeByVMEHash::~mafVisitorFindSceneNodeByVMEHash() {
}

void mafVisitorFindSceneNodeByVMEHash::visit(mafObjectBase *object) {
    mafSceneNode *n = qobject_cast<mafSceneNode *>(object);
    if(n != NULL) {
        mafVME *vme = n->vme();
        if(vme != NULL) {
            if(m_HashCode == vme->objectHash()) {
                m_SceneNode = n;
            }
        }
    }
}
