/*
 *  mafVTKInteratorPicker.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 12/12/10.
 *  Copyright 2009 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVTKINTERATORPICKER_H
#define MAFVTKINTERATORPICKER_H

// Includes list
#include "mafPluginVTKDefinitions.h"

class vtkAlgorithmOutput;

namespace mafPluginVTK {

/**
Class name: mafVTKInteratorPicker
This class represent an interactor implementing a picking operation.
*/

class MAFPLUGINVTKSHARED_EXPORT mafVTKInteratorPicker : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:

    enum PARAMETRIC_SURFACE_TYPE_ID
      {
        PARAMETRIC_SPHERE = 0,
        PARAMETRIC_CONE,
        PARAMETRIC_CYLINDER,
        PARAMETRIC_CUBE,
        PARAMETRIC_PLANE,
        PARAMETRIC_ELLIPSOID,
      };

    enum ID_ORIENTATION_AXIS
      {
        ID_X_AXIS = 0,
        ID_Y_AXIS,
        ID_Z_AXIS,
      };

    /// Object constructor.
    mafVTKInteratorPicker(const mafString code_location = "");

signals:
    /// Signal for VME picked.
    void vmePickSignal(double *picktPos, unsigned long,  mafCore::mafContainerInterface *actor, QEvent *e);


private slots:
    /// Called when a VME has been picked.
    void vmePick(double *pickPos, unsigned long, mafCore::mafContainerInterface *actor, QEvent * e);

    /// Set type of surface to create.
    void setGeometryType(int parametricSurfaceTypeID);

    /// Set the radius for the parametric sphere.
    void setSphereRadius(double radius);

    /// Set the Phi resolution for the parametric sphere.
    void setSpherePhiResolution(double spherePhiRes);

    /// Set the Theta resolution for the parametric sphere.
    void setThetaResolution(double sphereTheRes);

    /// Set the height of the parametric cone.
    void setConeHeight(double coneHeight);

    /// Set the radius for the base of the parametric cone.
    void setConeRadius(double coneRadius);

    /// Set the capping of the parametric cone.
    void setConeCapping(int coneCapping);

    /// Set the resolution of the parametric cone.
    void setConeResolution(double coneRes);

    /// Set the radius for the base of the parametric cylinder.  
    void setCylinderHeight(double cylinderHeight);

    /// Set the radius for the base of the parametric cylinder.
    void setCylinderRadius(double cylinderRadius);

    /// Set the radius for the base of the parametric cylinder.
    void setCylinderResolution(double cylinderRes);

    /// Set the radius for the base of the parametric cone.
    void setCubeSide(double XLength, double YLength, double ZLength);

    /// Set the radius for the base of the parametric ellipsoid.
    void setEllipsoidRadius(double ellipsoidYLenght);

    /// Set the radius for the base of the parametric ellipsoid.
    void setEllipsoidPhiResolution(double ellipsoidPhiRes);

    /// Set the radius for the base of the parametric ellipsoid.
    void setEllipsoidThetaResolution(double ellipsoidTheRes);

    /// Return the surface created.
    vtkAlgorithmOutput *GetSurfaceOutput();


private:
    /// Initialize connection between signals and slots.
    void initializeConnections();

    /// Create choosen surface.
    void internalUpdate();

    vtkAlgorithmOutput *m_SurfaceOutput;

    int m_GeometryType;
    double m_SphereRadius;
    double m_SpherePhiRes;
    double m_SphereTheRes;
    double m_ConeHeight;
    double m_ConeRadius;
    int m_ConeCapping;
    double m_ConeRes;
    int m_ConeOrientationAxis;
    double m_CylinderHeight;
    double m_CylinderRadius;
    double m_CylinderRes;
    int m_CylinderOrientationAxis;
    double m_CubeXLength;
    double m_CubeYLength;
    double m_CubeZLength;
    double m_EllipsoidXLenght;
    double m_EllipsoidYLenght;
    double m_EllipsoidZLenght;
    double m_EllipsoidPhiRes;
    double m_EllipsoidTheRes;
    double *m_Center;
    int m_EllipsoidOrientationAxis;


};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline vtkAlgorithmOutput *mafVTKInteratorPicker::GetSurfaceOutput() {
    return m_SurfaceOutput;
}

} // namespace mafPluginVTK

#endif // MAFVTKINTERATORPICKER_H
