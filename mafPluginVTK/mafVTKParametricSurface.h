/*
 *  mafVTKParametricSurface.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 19/01/11.
 *  Copyright 2009 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVTKPARAMETRICSURFACE_H
#define MAFVTKPARAMETRICSURFACE_H

// Includes list
#include "mafPluginVTKDefinitions.h"

// Foundation Class forwarding list
class vtkAlgorithmOutput;

namespace mafPluginVTK {

/**
Class name: mafVTKParametricSurface
This is the base class of different type of parametric surface classes.
*/

class MAFPLUGINVTKSHARED_EXPORT mafVTKParametricSurface : public mafCore::mafObject
{
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObject);

public:
    mafVTKParametricSurface(const mafString code_location = "");

    /// Set the center of the parametric surface.
    void setCenter(double *center);

    /// Get the center of the parametric surface.
    double *center();

    /// Return the output of the polydata created.
    virtual vtkAlgorithmOutput* output();

    /// Update surface with parameters.
    virtual void updateSurface() = 0;

protected:
    double m_Center[3]; ///< Center of the parametric surface.
    vtkAlgorithmOutput *m_Output; ///< Output of the parametric surface.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafVTKParametricSurface::setCenter(double *center) {
    int i = 0;
    for (; i<3; i++) {
        m_Center[i] = center[i];
    }
}

inline double* mafVTKParametricSurface::center() {
    return m_Center;
}

}

#endif // MAFVTKPARAMETRICSURFACE_H
