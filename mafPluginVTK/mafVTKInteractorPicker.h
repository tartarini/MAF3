/*
 *  mafVTKInteractorPicker.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 12/12/10.
 *  Copyright 2009 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVTKINTERACTORPICKER_H
#define MAFVTKINTERACTORPICKER_H

// Includes list
#include "mafPluginVTKDefinitions.h"

// Foundation Class forwarding list
class vtkAlgorithmOutput;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkActor;
class vtkAppendPolyData;

namespace mafPluginVTK {

/**
Class name: mafVTKInteractorPicker
This class represent an interactor implementing a picking operation.
*/

class MAFPLUGINVTKSHARED_EXPORT mafVTKInteractorPicker : public mafResources::mafInteractor {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafInteractor);

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
    mafVTKInteractorPicker(const mafString code_location = "");

    /// Get output of the pipe.
    mafCore::mafContainerInterface *output();

signals:
    /// Signal for VME picked.
    void vmePickSignal(double *picktPos, unsigned long,  mafCore::mafContainerInterface *actor, QEvent *e);

    /// Store last picking point and allow to set a new picking point
    void nextPickSignal();

    /// Remove last picking position stored
    void unDoPickSignal();

public slots:
    /// Set type of surface to create.
    void setGeometryType(int parametricSurfaceTypeID);


private slots:
    /// Called when a VME has been picked.
    void vmePick(double *pickPos, unsigned long, mafCore::mafContainerInterface *actor, QEvent * e);

    /// Called when a new pick is required.
    void nextPick();

    /// Called to remove the last pick.
    //void unDoPick();

    /// Called when the operation terminates correctly.
    void OK();



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

    /// Initialize and create the pipeline.
    void createPipe();

    /// Set scalar value to input vtkPolyData.
    void setScalarValue(vtkPolyData *data, double scalarValue);

    vtkAlgorithmOutput *m_MarkerOutput;
    vtkAlgorithmOutput *m_TmpMarkerOutput;
    vtkAlgorithmOutput *m_LastMarkerOutput;

    int m_MarkerIndex;
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
    bool m_IsFirstPick;

    vtkPolyDataMapper  *m_Mapper; ///< Class that maps polygonal data.

    vtkAppendPolyData *m_AppendData; /// Bunch of surfaces.

    mafCore::mafContainer<vtkActor> m_Actor; ///< Output container.

    mafList<double*> m_PointList;

    mafCore::mafContainerInterface *m_Output; ///< Output for visual pipe.


};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline vtkAlgorithmOutput *mafVTKInteractorPicker::GetSurfaceOutput() {
    return m_MarkerOutput;
}

inline mafCore::mafContainerInterface *mafVTKInteractorPicker::output() {
    return m_Output;
}

} // namespace mafPluginVTK

#endif // MAFVTKINTERATORPICKER_H
