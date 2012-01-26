/*
 *  mafSceneNode.h
 *  mafResources
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 24/03/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSCENENODE_H
#define MAFSCENENODE_H

#include "mafResourcesDefinitions.h"

namespace mafResources {

// Class forwarding list
class mafPipeVisual;
class mafVME;
class mafView;

/**
Class name: mafSceneNode
This class represents a scene node which owns the pointer to the mafVME and the corresponding Visual Pipe.
*/
class MAFRESOURCESSHARED_EXPORT mafSceneNode : public mafCore::mafObject {
    Q_OBJECT
    Q_PROPERTY(bool visibility READ visibility WRITE setVisibility)
    Q_PROPERTY(unsigned int visualizationStatus READ visualizationStatus WRITE setVisualizationStatus)
    Q_PROPERTY(QString VMEName READ VMEName WRITE setVMEName)
    Q_PROPERTY(QString dataHash READ dataHash)
    Q_PROPERTY(unsigned int visibilityPolicy READ visibilityPolicy WRITE setVisibilityPolicy)
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObject);

public:
    /// Object constructor.
    mafSceneNode(const QString code_location = "");

    /// Object constructor (Used in test suite)
    mafSceneNode(mafVME *vme, QObject *graphicObject, const QString visualPipeType, const QString code_location = "");
    
    /// Initialize scene node information according to the assigned VME.
    /*virtual*/ bool initialize();

    /// Set the parent of the current node.
    virtual void setParentNode(const mafSceneNode *parent);

    /// Return the parent node.
    mafSceneNode *parentNode() const;

    /// Return the type name of the pipe.
    QString visualPipeType() const;
    
    /// Set visual pipe to the scene node
    void setVisualPipe(QString visualPipeType);

    /// Return the inner visual pipe.
    mafPipeVisual *visualPipe() const;
    
    /// Return the inner VME.
    mafVME *vme() const;

    /// Set name of VME contained by scene node.
    void setVMEName(QString name);

    /// Return name of contained VME.
    QString VMEName();
    
    /// Return hash of the current Data (VME).
    QString dataHash() const;

    /// Set visibility policy of the scene node
    void setVisibilityPolicy(unsigned int visibilityPolicy);
    
    /// Return visibility policy of the scene node
    unsigned int visibilityPolicy() const;

protected:
    /// Object destructor.
    /* virtual */ ~mafSceneNode();

    /// Return the visibility status
    bool visibility() const;

    /// Return the visibility status
    unsigned int visualizationStatus();
    
    QObject *m_GraphicObject; ///< Render device.

private:
    /// Create instance of visual pipe.
    bool createVisualPipe();

    mafVME *m_VME; ///< Represent the data object.
    QString m_VisualPipeType; ///< type name of the current visual pipe
    mafPipeVisual *m_VisualPipe;  ///< represents the renderable object inside the scene node
    bool m_Visibility; ///< Contains the visibility status of the owned object/s.
    mafVisualizationStatus m_VisualizationStatus; ///< contains the visibility status for that scene node.
    mafVisibilityPolicy m_VisibilityPolicy; ///< Visibility policy determines in which way, on show/hide , the visual pipe will be treated.
    mafSceneNode *m_ParentNode; ///< Parent node.

    friend class mafView;

Q_SIGNALS:
    /// Alert the view that the node can be deleted because its inner VME has been destroyed.
    void destroyNode();

public Q_SLOTS:
    QObject *dataObject();
    
    /// Set the visibility of its rendering scene.
    virtual void setVisibility(bool visible);

    /// Set the visualization status property.
    virtual void setVisualizationStatus(bool visualizationStatus);
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline QString mafSceneNode::visualPipeType() const {
    return m_VisualPipeType;
}    

inline mafPipeVisual *mafSceneNode::visualPipe() const {
    return m_VisualPipe;
}

inline mafVME *mafSceneNode::vme() const {
    return m_VME;
}

inline bool mafSceneNode::visibility() const {
    return m_Visibility;
}

inline unsigned int mafSceneNode::visibilityPolicy() const {
    return m_VisibilityPolicy;
}    

inline mafSceneNode *mafSceneNode::parentNode() const {
    return m_ParentNode;
}

} //namespace mafResources

#endif // MAFSCENENODE_H
