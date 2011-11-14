/*
 *  mafPipeDataSliceSurface.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 14/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPIPEDATASLICESURFACE_H
#define MAFPIPEDATASLICESURFACE_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafPipeData.h>
#include <mafProxy.h>
#include <vtkSmartPointer.h>
#include <vtkAlgorithmOutput.h>

// Foundation Class forwarding list
class vtkPlane;
class vtkCutter;

namespace mafPluginVTK {

// Class forwarding list

/**
 Class name: mafPipeDataSliceSurface
 This class allows you to make a slice on input vtkPolyData.
 */
class MAFPLUGINVTKSHARED_EXPORT mafPipeDataSliceSurface : public mafResources::mafPipeData {
    Q_OBJECT

    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipeData);

public:
    /// Object constructor.
    mafPipeDataSliceSurface(const QString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

    /// Set the origin of the slice
    void setSliceOrigin(double *origin);

    /// Return the slice origin.
    double *sliceOrigin();

    /// Set the normal of the slice
    void setNormal(double *normal);

    /// Return the slice normal.
    double *normal();

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);

protected:
    /// Object destructor.
    /* virtual */ ~mafPipeDataSliceSurface();

private:
    double m_SliceOrigin[3]; ///< Origin of the slice.
    double m_Normal[3]; ///< Normal vector of the slice.

	vtkSmartPointer<vtkPlane>  m_Plane;  ///< Cutting plane implicit function.
    vtkSmartPointer<vtkCutter> m_Cutter; ///< Cutter fo the input vtkPolyData.
    mafCore::mafProxy<vtkAlgorithmOutput> m_OutputValue; ///< Output value coming from the elaboration data pipe.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafPipeDataSliceSurface::setSliceOrigin(double *origin) {
    REQUIRE(origin);

    m_SliceOrigin[0] = origin[0];
    m_SliceOrigin[1] = origin[1];
    m_SliceOrigin[2] = origin[2];
}

inline double *mafPipeDataSliceSurface::sliceOrigin() {
    return m_SliceOrigin;
}

inline void mafPipeDataSliceSurface::setNormal(double *normal) {
    REQUIRE(normal);

    m_Normal[0] = normal[0];
    m_Normal[1] = normal[1];
    m_Normal[2] = normal[2];
}

inline double *mafPipeDataSliceSurface::normal() {
    return m_Normal;
}

} // namespace mafPluginVTK

#endif // MAFPIPEDATASLICESURFACE_H
