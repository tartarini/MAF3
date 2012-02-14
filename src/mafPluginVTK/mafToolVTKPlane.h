/*
 *  mafToolVTKPlane.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 19/1/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTOOLVTKPLANE_H
#define MAFTOOLVTKPLANE_H

// Includes list
#include "mafPluginVTK_global.h"
#include "mafToolVTK.h"
#include <mafBounds.h>
#include <mafPoint.h>

#include <vtkSmartPointer.h>

class vtkImplicitPlaneWidget2;

namespace mafPluginVTK {

/**
 Class name: mafToolVTKPlane
 This is the tool representing an infinite plane.
 */
class MAFPLUGINVTKSHARED_EXPORT mafToolVTKPlane : public mafToolVTK {
    Q_OBJECT
    Q_PROPERTY(mafCore::mafPointPointer origin READ origin WRITE setOrigin);
    Q_PROPERTY(mafCore::mafPointPointer normal READ normal WRITE setNormal);
    Q_PROPERTY(mafCore::mafBoundsPointer voi READ voi WRITE setVOI);
    
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafToolVTK);

public Q_SLOTS:
    /// Called when the graphic object has been initialized ready to use
    /*virtual*/ void graphicObjectInitialized();

public:
     /// Object constructor.
    mafToolVTKPlane(const QString code_location = "");

    /// Allow to take the tool to the initial conditions.
    /*virtual*/ void resetTool();

    /// Allow to assign bounds into which place the plane widget.
    /** The widget will move itself inside the volume of interest (VOI) defined 
    by the bounds passed as argument.*/
    void setVOI(mafCore::mafBounds *bounds);

    /// Return the volume of interest of the widget.
    mafCore::mafBounds *voi() const;

    /// Return the origin coordinate.
    mafCore::mafPoint *origin() const;

    /// Assign the origin coordinate.
    void setOrigin(mafCore::mafPoint *o);

    /// Return the normal coordinate.
    mafCore::mafPoint *normal() const;

    /// Assign the normal coordinate.
    void setNormal(mafCore::mafPoint *n);

protected:
    /// Object destructor.
    /* virtual */ ~mafToolVTKPlane();

private:
    vtkSmartPointer<vtkImplicitPlaneWidget2> m_PlaneWidget; ///> Plane widget.

    mafCore::mafPoint *m_Origin; ///< Origin of the plane tool.
    mafCore::mafPoint *m_Normal; ///< Normal of the plane tool.
    mafCore::mafBounds *m_VOI; ///< Volume of interest in which place the plane widget.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafCore::mafPoint *mafToolVTKPlane::origin() const {
    return m_Origin;
}

inline mafCore::mafPoint *mafToolVTKPlane::normal() const {
    return m_Normal;
}

inline mafCore::mafBounds *mafToolVTKPlane::voi() const {
    return m_VOI;
}

} //namespace mafPluginVTK

#endif // MAFTOOLVTKPLANE_H
