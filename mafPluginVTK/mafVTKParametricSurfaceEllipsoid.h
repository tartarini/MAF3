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
class MAFPLUGINVTKSHARED_EXPORT mafVTKParametricSurfaceEllipsoid : public mafPluginVTK::mafVTKParametricSurface {
    Q_OBJECT
    Q_PROPERTY(double ellipsoidRadius READ ellipsoidRadius WRITE setEllipsoidRadius)
    Q_PROPERTY(double ellipsoidPhiRes READ ellipsoidPhiRes WRITE setEllipsoidPhiRes)
    Q_PROPERTY(double ellipsoidTheRes READ ellipsoidTheRes WRITE setEllipsoidTheRes)
    Q_PROPERTY(double ellipsoidXLength READ ellipsoidXLength WRITE setEllipsoidXLength)
    Q_PROPERTY(double ellipsoidYLength READ ellipsoidYLength WRITE setEllipsoidYLength)
    Q_PROPERTY(double ellipsoidZLength READ ellipsoidZLength WRITE setEllipsoidZLength)
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafVTKParametricSurface);

public:
    /// Object constructor.
    mafVTKParametricSurfaceEllipsoid(const QString code_location = "");

public slots:

    /// Update surface with parameters.
    /*virtual*/ void updateSurface();

protected:
    /// Object destructor.
    /*virtual*/ ~mafVTKParametricSurfaceEllipsoid();

private slots:

    /// Set the radius of the parametric ellipsoid.
    void setEllipsoidRadius(double ellipsoidRadius);

    /// Set the resolution of the parametric ellipsoid.
    void setEllipsoidPhiRes(double ellipsoidPhiRes);

    /// Set the resolution of the parametric ellipsoid.
    void setEllipsoidTheRes(double ellipsoidTheRes);

    /// Set the X length of the parametric ellipsoid.
    void setEllipsoidXLength(double ellipsoidXLength);

    /// Set the X length of the parametric ellipsoid.
    void setEllipsoidYLength(double ellipsoidYLength);

    /// Set the X length of the parametric ellipsoid.
    void setEllipsoidZLength(double ellipsoidZLength);

    /// Get the radius of the parametric ellipsoid.
    double ellipsoidRadius();

    /// get the resolution of the parametric ellipsoid.
    double ellipsoidPhiRes();

    /// get the resolution of the parametric ellipsoid.
    double ellipsoidTheRes();

    /// Get the X length of the parametric ellipsoid.
    double ellipsoidXLength();

    /// Get the Y length of the parametric ellipsoid.
    double ellipsoidYLength();

    /// Get the Z length of the parametric ellipsoid.
    double ellipsoidZLength();


private:
    vtkSphereSource *m_EllipsoidSource; ///< Polydata representig a ellipsoid.
    vtkTransformPolyDataFilter *m_Ptf; ///< Filter to scale sphere source.
    vtkTransform *m_Transofrm; ///< Filter to transform sphere source.

    double m_EllipsoidRadius; ///< Radius of the ellipsoid.
    double m_EllipsoidPhiRes; ///< Ellipsoid resolution.
    double m_EllipsoidTheRes; ///< Ellipsoid resolution.
    double m_EllipsoidXLength; ///< X length of the ellipsoid.
    double m_EllipsoidYLength; ///< Y length of the ellipsoid.
    double m_EllipsoidZLength; ///< Z length of the ellipsoid.

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

inline void mafVTKParametricSurfaceEllipsoid::setEllipsoidXLength(double ellipsoidXLength){
    m_EllipsoidXLength = ellipsoidXLength;
}

inline void mafVTKParametricSurfaceEllipsoid::setEllipsoidYLength(double ellipsoidYLength){
    m_EllipsoidYLength = ellipsoidYLength;
}

inline void mafVTKParametricSurfaceEllipsoid::setEllipsoidZLength(double ellipsoidZLength){
    m_EllipsoidZLength = ellipsoidZLength;
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

inline double mafVTKParametricSurfaceEllipsoid::ellipsoidXLength(){
    return m_EllipsoidXLength;
}

inline double mafVTKParametricSurfaceEllipsoid::ellipsoidYLength(){
    return m_EllipsoidYLength;
}

inline double mafVTKParametricSurfaceEllipsoid::ellipsoidZLength(){
    return m_EllipsoidZLength;
}

}

#endif // MAFVTKPARAMETRICSURFACEELLIPSOID_H
