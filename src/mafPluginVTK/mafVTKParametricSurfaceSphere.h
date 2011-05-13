/*
 *  mafVTKParametricSurfaceSphere.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 19/01/11.
 *  Copyright 2011 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVTKPARAMETRICSURFACESPHERE_H
#define MAFVTKPARAMETRICSURFACESPHERE_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafVTKParametricSurface.h"

// Foundation Class forwarding list
class vtkAlgorithmOutput;
class vtkSphereSource;

namespace mafPluginVTK {

/**
Class name: mafVTKParametricSurfaceSphere
This class represent a VTK sphere surface with modificable parametrs.
*/
class MAFPLUGINVTKSHARED_EXPORT mafVTKParametricSurfaceSphere : public mafPluginVTK::mafVTKParametricSurface {
    Q_OBJECT
    Q_PROPERTY(double sphereRadius READ sphereRadius WRITE setSphereRadius)
    Q_PROPERTY(double spherePhiRes READ spherePhiResolution WRITE setSpherePhiResolution)
    Q_PROPERTY(double sphereThetaRes READ sphereThetaResolution WRITE setSphereThetaResolution)
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafVTKParametricSurface);

public:
    /// Object constructor.
    mafVTKParametricSurfaceSphere(const QString code_location = "");

public slots:

    /// Update surface with parameters.
    /*virtual*/ void updateSurface();

protected:
    /// Object destructor.
    /*virtual*/ ~mafVTKParametricSurfaceSphere();

private slots:
    /// Set the radius for the parametric sphere.
    void setSphereRadius(double radius);

    /// Set the Phi resolution for the parametric sphere.
    void setSpherePhiResolution(double spherePhiRes);

    /// Set the Theta resolution for the parametric sphere.
    void setSphereThetaResolution(double sphereThetaRes);

    /// Get the radius for the parametric sphere.
    double sphereRadius();

    /// Get the Phi resolution for the parametric sphere.
    double spherePhiResolution();

    /// Get the Theta resolution for the parametric sphere.
    double sphereThetaResolution();


private:
    vtkSphereSource *m_SphereSource; ///< Polydata representig a sphere.

    double m_SphereRadius; ///< Radius of the sphere;
    double m_SpherePhiRes; ///< Sphere Phi resolution;
    double m_SphereThetaRes; ///< Sphere Phi resolution;
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafVTKParametricSurfaceSphere::setSphereRadius(double sphereRadius){
    m_SphereRadius = sphereRadius;
}

inline void mafVTKParametricSurfaceSphere::setSpherePhiResolution(double spherePhiRes){
    m_SpherePhiRes = spherePhiRes;
}

inline void mafVTKParametricSurfaceSphere::setSphereThetaResolution(double sphereThetaRes){
    m_SphereThetaRes = sphereThetaRes;
}

inline double mafVTKParametricSurfaceSphere::sphereRadius(){
    return m_SphereRadius;
}

inline double mafVTKParametricSurfaceSphere::spherePhiResolution(){
    return m_SpherePhiRes;
}

inline double mafVTKParametricSurfaceSphere::sphereThetaResolution(){
    return m_SphereThetaRes;
}

}

#endif // MAFVTKPARAMETRICSURFACESPHERE_H
