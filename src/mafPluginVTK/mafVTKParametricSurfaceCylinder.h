/*
 *  mafVTKParametricSurfaceCylinder.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 19/01/11.
 *  Copyright 2011 SCS-B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVTKPARAMETRICSURFACECYLINDER_H
#define MAFVTKPARAMETRICSURFACECYLINDER_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafVTKParametricSurface.h"

// Foundation Class forwarding list
class vtkAlgorithmOutput;
class vtkCylinderSource;

namespace mafPluginVTK {

/**
Class name: mafVTKParametricSurfaceCylinder
This class represent a VTK cylinder surface with modificable parametrs.
*/
class MAFPLUGINVTKSHARED_EXPORT mafVTKParametricSurfaceCylinder : public mafPluginVTK::mafVTKParametricSurface {
    Q_OBJECT
    Q_PROPERTY(double cylinderHeight READ cylinderHeight WRITE setConeHeight)
    Q_PROPERTY(double cylinderRadius READ cylinderRadius WRITE setConeRadius)
    Q_PROPERTY(double cylinderRes READ cylinderRes WRITE setConeRes)
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafVTKParametricSurface);

public:
    /// Object constructor.
    mafVTKParametricSurfaceCylinder(const QString code_location = "");

public Q_SLOTS:

    /// Update surface with parameters.
    /*virtual*/ void updateSurface();

protected:
    /// Object destructor.
    /*virtual*/ ~mafVTKParametricSurfaceCylinder();

private Q_SLOTS:
    /// Set the height of the parametric cylinder.
    void setConeHeight(double cylinderHeight);

    /// Set the radius of the parametric cylinder.
    void setConeRadius(double cylinderRadius);

    /// Set the resolution of the parametric cylinder.
    void setConeRes(double cylinderRes);

    /// Get the height of the parametric cylinder.
    double cylinderHeight();

    /// Get the radius of the parametric cylinder.
    double cylinderRadius();

    /// get the resolution of the parametric cylinder.
    double cylinderRes();

private:
    vtkCylinderSource *m_CylinderSource; ///< Polydata representig a cylinder.

    double m_CylinderHeight; ///< Height of the cylinder.
    double m_CylinderRadius; ///< Radius of the cylinder.
    double m_CylinderRes; ///< Cone resolution.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafVTKParametricSurfaceCylinder::setConeHeight(double cylinderHeight){
    m_CylinderHeight = cylinderHeight;
}

inline void mafVTKParametricSurfaceCylinder::setConeRadius(double cylinderRadius){
    m_CylinderRadius = cylinderRadius;
}

inline void mafVTKParametricSurfaceCylinder::setConeRes(double cylinderRes){
    m_CylinderRes = cylinderRes;
}

inline double mafVTKParametricSurfaceCylinder::cylinderHeight(){
    return m_CylinderHeight;
}

inline double mafVTKParametricSurfaceCylinder::cylinderRadius(){
    return m_CylinderRadius;
}

inline double mafVTKParametricSurfaceCylinder::cylinderRes(){
    return m_CylinderRes;
}

}

#endif // MAFVTKPARAMETRICSURFACECYLINDER_H
