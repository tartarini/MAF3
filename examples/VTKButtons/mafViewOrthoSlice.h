/*
 *  mafViewOrthoSlice.h
 *  VTKButtons
 *
 *  Created by Paolo Quadrani on 27/10/11.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVIEWORTHOSLICE_H
#define MAFVIEWORTHOSLICE_H

#include <mafViewCompound.h>
#include <mafSceneNode.h>
#include "mafToolVTKOrthoPlane.h"
#include <mafVME.h>
#include <mafPoint.h>

class mafOrthoSlice;

/**
 Class name: mafViewOrthoSlice
 mafView compound with 4 panels containing 4 rendering windows.
*/
class mafViewOrthoSlice : public mafResources::mafViewCompound {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafViewCompound);

public Q_SLOTS:
    /// calling this when widget has been moved.
    void widgetUpdatePosition();

    /// calling this when gui sliders have been moved.
    void guiUpdatePosition(double *pos);
    
    //////////////////////////////////////////////////////////////////////////
    // Delegate methods
    //////////////////////////////////////////////////////////////////////////
    /// Get originZ value;
    mafCore::mafPointPointer origin();
    //////////////////////////////////////////////////////////////////////////

public:
    /// Object constructor.
    mafViewOrthoSlice(const QString code_location = "");

    /// Show scene node passed as argument.
    /*virtual*/ void showSceneNode(mafResources::mafSceneNode *node, bool show = true);

    /// Return true or false depending if the delegate class wants that the caller execute its own code or skip it.
    /*virtual*/ bool shouldExecuteLocalCode();

protected:
    /// Object destructor.
    /* virtual */ ~mafViewOrthoSlice();

private:
    /// internal slice update.
    void updateSlice();
    
    mafOrthoSlice *m_GUI; ///< GUI widget.
    mafCore::mafPoint *m_SlicePosition; ///< Current slice position.
    QList<mafToolVTKOrthoPlane *> m_OrthoPlaneTool;

    /// Initialize the plane tools
    void addPlaneToolsToHandler();
    
    mafCore::mafObjectsList m_VisibleVMEsList;
};

#endif // MAFVIEWORTHOSLICE_H
