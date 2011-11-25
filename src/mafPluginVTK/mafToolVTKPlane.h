/*
 *  mafToolVTKPlane.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 25/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTOOLVTKPLANE_H
#define MAFTOOLVTKPLANE_H

// Includes list
#include "mafPluginVTK_global.h"
#include "mafToolVTK.h"

#include <vtkSmartPointer.h>

class vtkImplicitPlaneWidget2;
class vtkPlane;

namespace mafPluginVTK {

/**
 Class name: mafToolVTKPlane
 This is the tool representing an infinite plane.
 */
class MAFPLUGINVTKSHARED_EXPORT mafToolVTKPlane : public mafToolVTK {
    Q_OBJECT
    
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafToolVTK);

public Q_SLOTS:
    /// Called when the graphic object has been initialized ready to use
    /*virtual*/ void graphicObjectInitialized();

public:
     /// Object constructor.
    mafToolVTKPlane(const QString code_location = "");

    /// Assign the scene node to the tool so to have access to the VME and eventually its visual representation.
    /*virtual*/ void setSceneNode(mafResources::mafSceneNode *node);

protected:
    /// Object destructor.
    /* virtual */ ~mafToolVTKPlane();

    /// update visibility for actor or volume passed as parameter
    /*virtual*/ void updateVisibility();

private:
    vtkSmartPointer<vtkImplicitPlaneWidget2> m_PlaneWidget; ///> Plane widget.
    vtkPlane *m_Plane; ///< Implicit plane.
};

} //namespace mafPluginVTK

#endif // MAFTOOLVTKPLANE_H
