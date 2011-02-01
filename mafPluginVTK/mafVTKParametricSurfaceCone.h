/*
 *  mafVTKParametricSurfaceCone.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 19/01/11.
 *  Copyright 2011 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVTKPARAMETRICSURFACECONE_H
#define MAFVTKPARAMETRICSURFACECONE_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafVTKParametricSurface.h"

// Foundation Class forwarding list
class vtkAlgorithmOutput;
class vtkConeSource;

namespace mafPluginVTK {

/**
Class name: mafVTKParametricSurfaceCone
This class represent a VTK cone surface with modificable parametrs.
*/
class MAFPLUGINVTKSHARED_EXPORT mafVTKParametricSurfaceCone : public mafPluginVTK::mafVTKParametricSurface
{
    Q_OBJECT
    Q_PROPERTY(double coneHeight READ coneHeight WRITE setConeHeight)
    Q_PROPERTY(double coneRadius READ coneRadius WRITE setConeRadius)
    Q_PROPERTY(double coneRes READ coneRes WRITE setConeRes)
    Q_PROPERTY(int coneCapping READ coneCapping WRITE setConeCapping)
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafVTKParametricSurface);

public:
    /// Object constructor.
    mafVTKParametricSurfaceCone(const mafString code_location = "");

    /// Object destructor.
    ~mafVTKParametricSurfaceCone();

public slots:
    /// Set the height of the parametric cone.
    void setConeHeight(double coneHeight);

    /// Set the radius of the parametric cone.
    void setConeRadius(double coneRadius);

    /// Set the resolution of the parametric cone.
    void setConeRes(double coneRes);

    /// Set cap flag of the parametric cone.
    void setConeCapping(int coneCapping);

    /// Get the height of the parametric cone.
    double coneHeight();

    /// Get the radius of the parametric cone.
    double coneRadius();

    /// get the resolution of the parametric cone.
    double coneRes();

    /// Get cap flag of the parametric cone.
    int coneCapping();

    /// Update surface with parameters.
    /*virtual*/ void updateSurface();

private:
    vtkConeSource *m_ConeSource; ///< Polydata representig a cone.

    double m_ConeHeight; ///< Height of the cone.
    double m_ConeRadius; ///< Radius of the cone.
    double m_ConeRes; ///< Cone resolution.
    int m_ConeCapping; ///< Flag for cone cap.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafVTKParametricSurfaceCone::setConeHeight(double coneHeight){
    m_ConeHeight = coneHeight;
}

inline void mafVTKParametricSurfaceCone::setConeRadius(double coneRadius){
    m_ConeRadius = coneRadius;
}

inline void mafVTKParametricSurfaceCone::setConeRes(double coneRes){
    m_ConeRes = coneRes;
}

inline void mafVTKParametricSurfaceCone::setConeCapping(int coneCapping){
    m_ConeCapping = coneCapping;
}

inline double mafVTKParametricSurfaceCone::coneHeight(){
    return m_ConeHeight;
}

inline double mafVTKParametricSurfaceCone::coneRadius(){
    return m_ConeRadius;
}

inline double mafVTKParametricSurfaceCone::coneRes(){
    return m_ConeRes;
}

inline int mafVTKParametricSurfaceCone::coneCapping(){
    return m_ConeCapping;
}

}

#endif // MAFVTKPARAMETRICSURFACECONE_H
