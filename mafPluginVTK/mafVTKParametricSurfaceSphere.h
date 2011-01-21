/*
 *  mafVTKParametricSurfaceSphere.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 19/01/11.
 *  Copyright 2009 B3C.s All rights reserved.
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
Class name: mafVTKInteractorPicker
This class represent an interactor implementing a picking operation.
*/

    class MAFPLUGINVTKSHARED_EXPORT mafVTKParametricSurfaceSphere : public mafPluginVTK::mafVTKParametricSurface
{
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafVTKParametricSurface);

public:
    ///< Object constructor.
    mafVTKParametricSurfaceSphere(const mafString code_location = "");

    ///< Object destructor.
    ~mafVTKParametricSurfaceSphere();

public slots:
    /// Set the radius for the parametric sphere.
    void setSphereRadius(double radius);

    /// Set the Phi resolution for the parametric sphere.
    void setSpherePhiResolution(double spherePhiRes);

    /// Set the Theta resolution for the parametric sphere.
    void setThetaResolution(double sphereTheRes);

    /// Update surface with parameters.
    /*virtual*/ void updateSurface();

private:
    vtkSphereSource *m_SphereSource; ///< Polydata representig a sphere.

    double m_SphereRadius; ///< Radius of the sphere;
    double m_SpherePhiRes; ///< Sphere Phi resolution;
    double m_SphereTheRes; ///< Sphere Phi resolution;
};

}

#endif // MAFVTKPARAMETRICSURFACESPHERE_H
