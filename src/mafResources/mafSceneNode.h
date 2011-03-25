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
class mafPipeVisual;
class mafVME;

/**
Class name: mafSceneNode
This class represents a scene node which owns the pointer to the mafVME and the corresponding Visual Pipe.
*/
class MAFRESOURCESSHARED_EXPORT mafSceneNode : public mafCore::mafObject {
    Q_OBJECT
    Q_PROPERTY(bool visibility READ visibility WRITE setVisibility)
    Q_PROPERTY(bool canVisualize READ canVisualize WRITE setCanVisualize)
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObject);

public:
    /// Object constructor. USE CONSTRUCTOR BELOW WHICH TAKES VME AS INPUT.
    mafSceneNode(const QString code_location = "");

    /// Object constructor.
    mafSceneNode(mafVME *vme, mafPipeVisual *visual_pipe = NULL, const QString code_location = "");

    /// Set visual pipe to the scene node
    void setVisualPipe(QString visualPipeType);

    /// Return the inner visual pipe.
    mafPipeVisual *visualPipe() const;

    /// Return the inner vme.
    mafVME *vme() const;

    /// Allow to send the selection event for the owned VME.
    /*virtual*/ void setSelected(bool sel);

public slots:
    /// Set the visibility of its rendering scene.
    virtual void setVisibility(bool visible);

    /// Set the canVisualize property.
    virtual void setCanVisualize(bool canVisualize);

signals:
    /// Alert the view that the node can be deleted because its inner vme has been destroyed.
    void destroyNode();

protected:
    /// Object destructor.
    /* virtual */ ~mafSceneNode();

    /// Return the visibility status
    bool visibility() const;

    /// Return the canVisualize status
    bool canVisualize() const;

private slots:
    /// monitor the visual pipe deletation.
    void visualPipeDestroyed();

private:
    mafVME *m_VME; ///< Represent the data object.
    mafPipeVisual *m_VisualPipe;  ///< represente the renderable object inside the scene node
    bool m_Visibility; ///< Contains the visibility status of the owned object/s.
    bool m_CanVisualize; ///< Contains the canVisualize status of the owned object/s.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafPipeVisual *mafSceneNode::visualPipe() const {
    return m_VisualPipe;
}

inline mafVME *mafSceneNode::vme() const {
    return m_VME;
}

inline bool mafSceneNode::visibility() const {
    return m_Visibility;
}


} //namespace mafResources

#endif // MAFSCENENODE_H
