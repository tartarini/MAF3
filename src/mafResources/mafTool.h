/*
 *  mafTool.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 27/09/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTOOL_H
#define MAFTOOL_H

// Includes list
#include "mafPipeVisual.h"

namespace mafResources {

class mafSceneNode;

/**
 Class name: mafTool
 This is the base class for the MAF3 tools. Tools are used to represent widgets or more in general
 objects that are used to perform task but don't need to be persistent on disk.
 */
class MAFRESOURCESSHARED_EXPORT mafTool : public mafPipeVisual {
    Q_OBJECT
    Q_PROPERTY(bool followSelectedObject READ followSelectedObject WRITE setFollowSelectedObject)
    Q_PROPERTY(bool followSelectedObjectVisibility READ followSelectedObjectVisibility WRITE setFollowSelectedObjectVisibility)

    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipeVisual);

Q_SIGNALS:
    /// Emitted when the graphic object has been initialized ready to use
    void graphicObjectReady();

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);

    /// Called when the graphic object has been initialized ready to use
    virtual void graphicObjectInitialized();

public:
    /// Object constructor.
    mafTool(const QString code_location = "");

    /// Assign the scene node to the tool so to have access to the VME and eventually its visual representation.
    virtual void setSceneNode(mafSceneNode *node);

    /// Allow to take the tool to the initial conditions.
    virtual void resetTool();

    /// Return the value of the follow selection flag.
    bool followSelectedObject() const;

    /// Set the flag associated to the follow selected object property.
    void setFollowSelectedObject(bool follow = true);

    /// Return the value of the follow selection visibility flag.
    bool followSelectedObjectVisibility() const;

    /// Set the flag associated to the follow selected object visibility property.
    void setFollowSelectedObjectVisibility(bool follow = true);

protected:
    /// Object destructor.
    /* virtual */ ~mafTool();

    mafSceneNode *m_SceneNode; ///< Pointer to the scene node.
    bool m_FollowSelectedObject; ///< Flag indicating that the tool is attached to the selected object in the scene (the default is true).
    bool m_FollowSelectedObjectVisibility; ///< Flag indicating that the tool follows the visibility of the selected object.
};


/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline bool mafTool::followSelectedObject() const {
    return m_FollowSelectedObject;
}

inline void mafTool::setFollowSelectedObject(bool follow) {
    m_FollowSelectedObject = follow;
}

inline bool mafTool::followSelectedObjectVisibility() const {
    return m_FollowSelectedObjectVisibility;
}

inline void mafTool::setFollowSelectedObjectVisibility(bool follow) {
    m_FollowSelectedObjectVisibility = follow;
}

} //namespace mafResources

#endif // MAFTOOL_H
