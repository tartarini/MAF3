/*
 *  mafVisitorFindSceneNodeByVMEHash.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 10/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVISITORFINDSCENENODEBYVMEHASH_H
#define MAFVISITORFINDSCENENODEBYVMEHASH_H

// Includes list
#include "mafResourcesDefinitions.h"
#include <mafVisitorFindObjects.h>

namespace mafResources {

// Class forwarding list
class mafSceneNode;

/**
 Class name: mafVisitorFindSceneNodeByVMEHash
 This class define a visitor object that is able to find the mafSceneNode
 that contains the mafVME with the given hash code.
 @sa mafVisitor mafVisitorFindObjects mafVisitorFindObjectsByHashCode
 */
class MAFRESOURCESSHARED_EXPORT mafVisitorFindSceneNodeByVMEHash : public mafCore::mafVisitorFindObjects {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafVisitorFindObjects);

public:
    /// object constructor
    mafVisitorFindSceneNodeByVMEHash(const mafString code_location = "");

    /// overloaded object constructor
    mafVisitorFindSceneNodeByVMEHash(const mafString hash_code, const mafString code_location);

    /// Execute the defined operation on visited object passes as argument.
    /*virtual*/ void visit(mafObjectBase *object);

    /// Return the found mafSceneNode if any.
    mafSceneNode *sceneNode();

private:
    mafString m_HashCode;
    mafSceneNode *m_SceneNode; ///< Scene node found corresponding to the given VME hash
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafSceneNode *mafVisitorFindSceneNodeByVMEHash::sceneNode() {
    return m_SceneNode;
}

} // namespace mafResources

#endif // MAFVISITORFINDSCENENODEBYVMEHASH_H
