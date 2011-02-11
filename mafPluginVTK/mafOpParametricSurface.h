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
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafOperation);

public:
    /// Object constructor.
    mafOpParametricSurface(const mafString code_location = "");

    /// Object destructor.
    ~mafOpParametricSurface();

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

public slots:
    /// Set type of parametric surface used as marker.
    void setSurfaceType(int parametricSurfaceType);

    /// Set parameters of operation.
    virtual void setParameters(mafList<mafVariant> parameters);

    /// Execute the resource algorithm.
    virtual void execute();

    /// Terminate the execution.
    //virtual bool terminate();

private:
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
