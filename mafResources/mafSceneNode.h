/*
 *  mafSceneNode.h
 *  mafResources
 *
 *  Created by Daniele Giunchi on 24/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSCENENODE_H
#define MAFSCENENODE_H

#include "mafResourcesDefinitions.h"

namespace mafResources {

// Class forwarding list
class mafVisualPipe;
class mafVME;

/**
Class name: mafSceneNode
This class represents a scene node which owns the pointer to the mafVME and the corresponding Visual Pipe.
*/
class MAFRESOURCESSHARED_EXPORT mafSceneNode : public mafCore::mafObject {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObject);

public:
    /// Object constructor. USE CONSTRUCTOR BELOW WHICH TAKES VME AS INPUT.
    mafSceneNode(const mafString code_location = "");

    /// Object constructor.
    mafSceneNode(mafVME *vme, mafVisualPipe *visual_pipe = NULL, const mafString code_location = "");

    /// Set visual pipe to the scene node
    void setVisualPipe(mafString visualPipeType);

    /// Return the inner visual pipe.
    mafVisualPipe *visualPipe() const;

    /// Return the inner vme.
    mafVME *vme() const;

    /// Allow to send the selection event for the owned VME.
    /*virtual*/ void setSelected(bool sel);

signals:
    /// Alert the view that the node can be deleted because its inner vme has been destroyed.
    void destroyNode();

protected:
    /// Object destructor.
    /* virtual */ ~mafSceneNode();

private slots:
    /// monitor the visual pipe deletation.
    void visualPipeDestroyed();

private:
    mafVME *m_VME; ///< Represent the data object.
    mafVisualPipe *m_VisualPipe;  ///< represente the renderable object inside the scene node
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafVisualPipe *mafSceneNode::visualPipe() const {
    return m_VisualPipe;
}

inline mafVME *mafSceneNode::vme() const {
    return m_VME;
}

} //namespace mafResources

#endif // MAFSCENENODE_H
