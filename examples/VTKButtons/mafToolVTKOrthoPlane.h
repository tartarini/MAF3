/*
 *  mafToolVTKOrthoPlane.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 19/1/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTOOLVTKORTHOPLANE_H
#define MAFTOOLVTKORTHOPLANE_H

// Includes list
#include <mafToolVTK.h>
#include <mafBounds.h>
#include <mafPoint.h>

#include <vtkSmartPointer.h>

class vtkImplicitPlaneWidget2;

/**
 Class name: mafToolVTKOrthoPlane
 This is the tool represents 3 planes with their normals on the main axes.
 The planes can not rotate, but only move along their normal.
 */
class mafToolVTKOrthoPlane : public mafPluginVTK::mafToolVTK {
    Q_OBJECT
    Q_PROPERTY(mafCore::mafPoint *origin READ origin WRITE setOrigin);
    Q_PROPERTY(mafCore::mafBounds *voi READ voi WRITE setVOI);
    
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafToolVTK);

public Q_SLOTS:
    /// Called when the graphic object has been initialized ready to use
    /*virtual*/ void graphicObjectInitialized();

public:
     /// Object constructor.
    mafToolVTKOrthoPlane(const QString code_location = "");

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

protected:
    /// Object destructor.
    /* virtual */ ~mafToolVTKOrthoPlane();

private:
    vtkSmartPointer<vtkImplicitPlaneWidget2> m_PlaneWidgetX; ///> Plane widget.
    vtkSmartPointer<vtkImplicitPlaneWidget2> m_PlaneWidgetY; ///> Plane widget.
    vtkSmartPointer<vtkImplicitPlaneWidget2> m_PlaneWidgetZ; ///> Plane widget.

    mafCore::mafPoint *m_Origin; ///< Origin of the plane tool.
    mafCore::mafBounds *m_VOI; ///< Volume of interest in which place the plane widget.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafCore::mafPoint *mafToolVTKOrthoPlane::origin() const {
    return m_Origin;
}

inline mafCore::mafBounds *mafToolVTKOrthoPlane::voi() const {
    return m_VOI;
}

#endif // MAFTOOLVTKORTHOPLANE_H
