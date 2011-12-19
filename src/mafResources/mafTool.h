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

protected:
    /// Object destructor.
    /* virtual */ ~mafTool();

    mafSceneNode *m_SceneNode; ///< Pointer to the scene node.
};

} //namespace mafResources

#endif // MAFTOOL_H
