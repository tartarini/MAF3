/*
 *  mafVTKParametricSurfaceEllipsoid.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 19/01/11.
 *  Copyright 2011 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVTKPARAMETRICSURFACEELLIPSOID_H
#define MAFVTKPARAMETRICSURFACEELLIPSOID_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafVTKParametricSurface.h"

// Foundation Class forwarding list
class vtkAlgorithmOutput;
class vtkSphereSource;
class vtkTransformPolyDataFilter;
class vtkTransform;

namespace mafPluginVTK {

/**
Class name: mafVTKParametricSurfaceEllipsoid
This class represent a VTK ellipsoid surface with modificable parametrs.
*/

class MAFPLUGINVTKSHARED_EXPORT mafVTKParametricSurfaceEllipsoid : public mafPluginVTK::mafVTKParametricSurface
{
    Q_OBJECT
    Q_PROPERTY(double ellipsoidRadius READ ellipsoidRadius WRITE setEllipsoidRadius)
    Q_PROPERTY(double ellipsoidPhiRes READ ellipsoidPhiRes WRITE setEllipsoidPhiRes)
    Q_PROPERTY(double ellipsoidTheRes READ ellipsoidTheRes WRITE setEllipsoidTheRes)
    Q_PROPERTY(int ellipsoidXLenght READ ellipsoidXLenght WRITE setEllipsoidXLenght)
    Q_PROPERTY(int ellipsoidYLenght READ ellipsoidYLenght WRITE setEllipsoidYLenght)
    Q_PROPERTY(int ellipsoidZLenght READ ellipsoidZLenght WRITE setEllipsoidZLenght)
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafVTKParametricSurface);

public:
    ///< Object constructor.
    mafVTKParametricSurfaceEllipsoid(const mafString code_location = "");

    ///< Object destructor.
    ~mafVTKParametricSurfaceEllipsoid();

public slots:

    /// Set the radius of the parametric ellipsoid.
    void setEllipsoidRadius(double ellipsoidRadius);

    /// Set the resolution of the parametric ellipsoid.
    void setEllipsoidPhiRes(double ellipsoidPhiRes);

    /// Set the resolution of the parametric ellipsoid.
    void setEllipsoidTheRes(double ellipsoidTheRes);

    /// Set the X lenght of the parametric ellipsoid.
    void setEllipsoidXLenght(double ellipsoidXLenght);

    /// Set the X lenght of the parametric ellipsoid.
    void setEllipsoidYLenght(double ellipsoidYLenght);

    /// Set the X lenght of the parametric ellipsoid.
    void setEllipsoidZLenght(double ellipsoidZLenght);

    /// Get the radius of the parametric ellipsoid.
    double ellipsoidRadius();

    /// get the resolution of the parametric ellipsoid.
    double ellipsoidPhiRes();

    /// get the resolution of the parametric ellipsoid.
    double ellipsoidTheRes();

    /// Get the X lenght of the parametric ellipsoid.
    double ellipsoidXLenght();

    /// Get the Y lenght of the parametric ellipsoid.
    double ellipsoidYLenght();

    /// Get the Z lenght of the parametric ellipsoid.
    double ellipsoidZLenght();

    /// Update surface with parameters.
    /*virtual*/ void updateSurface();

private:
    vtkSphereSource *m_EllipsoidSource; ///< Polydata representig a ellipsoid.
    vtkTransformPolyDataFilter *m_Ptf; ///< Filter to scale sphere source.
    vtkTransform *m_Transofrm; ///< Filter to transform sphere source.

    double m_EllipsoidRadius; ///< Radius of the ellipsoid.
    double m_EllipsoidPhiRes; ///< Ellipsoid resolution.
    double m_EllipsoidTheRes; ///< Ellipsoid resolution.
    double m_EllipsoidXLenght; ///< X lenght of the ellipsoid.
    double m_EllipsoidYLenght; ///< Y lenght of the ellipsoid.
    double m_EllipsoidZLenght; ///< Z lenght of the ellipsoid.

};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafVTKParametricSurfaceEllipsoid::setEllipsoidRadius(double ellipsoidRadius){
    m_EllipsoidRadius = ellipsoidRadius;
}

inline void mafVTKParametricSurfaceEllipsoid::setEllipsoidPhiRes(double ellipsoidPhiRes){
    m_EllipsoidPhiRes = ellipsoidPhiRes;
}

inline void mafVTKParametricSurfaceEllipsoid::setEllipsoidTheRes(double ellipsoidThiRes){
    m_EllipsoidTheRes = ellipsoidThiRes;
}

inline void mafVTKParametricSurfaceEllipsoid::setEllipsoidXLenght(double ellipsoidXLenght){
    m_EllipsoidXLenght = ellipsoidXLenght;
}

inline void mafVTKParametricSurfaceEllipsoid::setEllipsoidYLenght(double ellipsoidYLenght){
    m_EllipsoidYLenght = ellipsoidYLenght;
}

inline void mafVTKParametricSurfaceEllipsoid::setEllipsoidZLenght(double ellipsoidZLenght){
    m_EllipsoidZLenght = ellipsoidZLenght;
}

inline double mafVTKParametricSurfaceEllipsoid::ellipsoidRadius(){
    return m_EllipsoidRadius;
}

inline double mafVTKParametricSurfaceEllipsoid::ellipsoidPhiRes(){
    return m_EllipsoidPhiRes;
}

inline double mafVTKParametricSurfaceEllipsoid::ellipsoidTheRes(){
    return m_EllipsoidTheRes;
}

inline double mafVTKParametricSurfaceEllipsoid::ellipsoidXLenght(){
    return m_EllipsoidXLenght;
}

inline double mafVTKParametricSurfaceEllipsoid::ellipsoidYLenght(){
    return m_EllipsoidYLenght;
}

inline double mafVTKParametricSurfaceEllipsoid::ellipsoidZLenght(){
    return m_EllipsoidZLenght;
}

}

#endif // MAFVTKPARAMETRICSURFACEELLIPSOID_H
