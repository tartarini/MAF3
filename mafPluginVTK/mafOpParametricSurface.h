/*
 *  mafOpParametricSurface.h
 *  SimpleApp
 *
 *  Created by Roberto Mucci on 10/02/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFOPPARAMETRICSURFACE_H
#define MAFOPPARAMETRICSURFACE_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafOperation.h>
#include "mafVTKParametricSurface.h"
#include "mafVTKParametricSurfaceSphere.h"
#include "mafVTKParametricSurfaceCube.h"
#include "mafVTKParametricSurfaceCone.h"
#include "mafVTKParametricSurfaceCylinder.h"
#include "mafVTKParametricSurfaceEllipsoid.h"


namespace mafPluginVTK {

/**
  Class Name: mafOpParametricSurface
  This class represent an operation that creates a parametric surface.
  */
class MAFPLUGINVTKSHARED_EXPORT mafOpParametricSurface : public mafResources::mafOperation {
    Q_OBJECT
    Q_PROPERTY(int parametricSurfaceType READ parametricSurfaceType WRITE setParametricSurfaceType)
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafOperation);

public:
    /// Object constructor.
    mafOpParametricSurface(const mafString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

public slots:
    /// Set type of parametric surface used as marker.
    void setParametricSurfaceType(int parametricSurfaceType);

    /// Return the type of parametric surface used as marker.
    int parametricSurfaceType();

    /// Set parameters of operation.
    virtual void setParameters(mafList<mafVariant> parameters);

    /// Execute the resource algorithm.
    virtual void execute();

    /// Select surface type.
    void on_parametricSurfaceType_currentChanged(int index);

    /// Set sphere radius.
    void on_sphereRadius_valueChanged(double d);

    /// Set sphere phi resolution.
    void on_spherePhiRes_valueChanged(double d);

    /// Set sphere theta resolution.
    void on_sphereThetaRes_valueChanged(double d);

    /// Set cube X length.
    void on_cubeXLength_valueChanged(double d);

    /// Set cube Y length.
    void on_cubeYLength_valueChanged(double d);

    /// Set cube Z length.
    void on_cubeZLength_valueChanged(double d);

    /// Set cone radius.
    void on_coneRadius_valueChanged(double d);

    /// Set cone height.
    void on_coneHeight_valueChanged(double d);

    /// Set cone res.
    void on_coneRes_valueChanged(double d);

    /// Set cone capping.
    void on_coneCapping_stateChanged(int state);

    /// Set cylinder radius.
    void on_cylinderRadius_valueChanged(double d);

    /// Set sphere height.
    void on_cylinderHeight_valueChanged(double d);

    /// Set sphere resolution.
    void on_cylinderRes_valueChanged(double d);

    /// Set ellipsoid radius.
    void on_ellipsoidRadius_valueChanged(double d);

    /// Set ellipsoid phi resolution.
    void on_ellipsoidPhiRes_valueChanged(double d);

    /// Set ellipsoid theta resolution.
    void on_ellipsoidThetaRes_valueChanged(double d);

    /// Set ellipsoid X length.
    void on_ellipsoidXLength_valueChanged(double d);

    /// Set ellipsoid Y length.
    void on_ellipsoidYLength_valueChanged(double d);

    /// Set ellipsoid Z length.
    void on_ellipsoidZLength_valueChanged(double d);


    /// Terminate the execution.
    //virtual bool terminate();

protected:
    /// Object destructor.
    /* virtual */ ~mafOpParametricSurface();

private:
    /// Create parametric surfaces.
    void initializeParametricSurfaces();

    int m_CurrentSurfaceIndex; ///< Index of the current parametric surface.
    int m_ParametricSurfaceType; ///< Kind of parametric surface.

    mafVTKParametricSurfaceSphere *m_ParametricSphere; ///< VTK parametric sphere.
    mafVTKParametricSurfaceCube *m_ParametricCube; ///< VTK parametric cube.
    mafVTKParametricSurfaceCone *m_ParametricCone; ///< VTK parametric cone.
    mafVTKParametricSurfaceCylinder *m_ParametricCylinder; ///< VTK parametric cylinder.
    mafVTKParametricSurfaceEllipsoid *m_ParametricEllipsoid; ///< VTK parametric ellipsoid.
    mafResources::mafVME *m_VME; ///< Output VME;
    mafResources::mafDataSet *m_DataSet; ///< Dataset of output VME;
    mafCore::mafContainer<vtkAlgorithmOutput> m_ParametricContainer; ///< output of the parametric sirface.
    mafList<mafVTKParametricSurface*> m_ParametricSurfaceList; ///< List of parametric surfaces.

};

///< Enumeration that define type of parametric sufaces.
enum {
    PARAMETRIC_SPHERE = 0,
    PARAMETRIC_CUBE,
    PARAMETRIC_CONE,
    PARAMETRIC_CYLINDER,
    PARAMETRIC_ELLIPSOID
};

}

#endif // MAFOPPARAMETRICSURFACE_H
