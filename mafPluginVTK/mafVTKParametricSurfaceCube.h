/*
 *  mafVTKParametricSurfaceCube.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 19/01/11.
 *  Copyright 2011 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVTKPARAMETRICSURFACECUBE_H
#define MAFVTKPARAMETRICSURFACECUBE_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafVTKParametricSurface.h"

// Foundation Class forwarding list
class vtkAlgorithmOutput;
class vtkCubeSource;

namespace mafPluginVTK {

/**
Class name: mafVTKParametricSurfaceCube
This class represent a VTK cube surface with modificable parametrs.
*/

    class MAFPLUGINVTKSHARED_EXPORT mafVTKParametricSurfaceCube : public mafPluginVTK::mafVTKParametricSurface
{
    Q_OBJECT
    Q_PROPERTY(double cubeXLength READ cubeXLength WRITE setCubeXLength)
    Q_PROPERTY(double cubeYLength READ cubeYLength WRITE setCubeYLength)
    Q_PROPERTY(double cubeZLength READ cubeZLength WRITE setCubeZLength)
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafVTKParametricSurface);

public:
    ///< Object constructor.
    mafVTKParametricSurfaceCube(const mafString code_location = "");

    ///< Object destructor.
    ~mafVTKParametricSurfaceCube();

public slots:
    /// Set the radius for the parametric sphere.
    void setCubeXLength(double XLength);

    /// Set the Phi resolution for the parametric sphere.
    void setCubeYLength(double YLength);

    /// Set the Theta resolution for the parametric sphere.
    void setCubeZLength(double ZLength);

    /// Get the radius for the parametric sphere.
    double cubeXLength();

    /// Get the Phi resolution for the parametric sphere.
    double cubeYLength();

    /// Get the Theta resolution for the parametric sphere.
    double cubeZLength();

    /// Update surface with parameters.
    /*virtual*/ void updateSurface();

private:
    vtkCubeSource *m_CubeSource; ///< Polydata representig a sphere.

    double m_CubeXLength; ///< Radius of the sphere;
    double m_CubeYLength; ///< Sphere Phi resolution;
    double m_CubeZLength; ///< Sphere Phi resolution;
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafVTKParametricSurfaceCube::setCubeXLength(double XLength){
    m_CubeXLength = XLength;
}

inline void mafVTKParametricSurfaceCube::setCubeYLength(double YLength){
    m_CubeYLength = YLength;
}

inline void mafVTKParametricSurfaceCube::setCubeZLength(double ZLength){
    m_CubeZLength = ZLength;
}

inline double mafVTKParametricSurfaceCube::cubeXLength(){
    return m_CubeXLength;
}

inline double mafVTKParametricSurfaceCube::cubeYLength(){
    return m_CubeYLength;
}

inline double mafVTKParametricSurfaceCube::cubeZLength(){
    return m_CubeZLength;
}

}

#endif // MAFVTKPARAMETRICSURFACESPHERE_H

