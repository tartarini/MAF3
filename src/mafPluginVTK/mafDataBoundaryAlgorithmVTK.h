/*
 *  mafDataBoundaryAlgorithm.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 10/11/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFDATABOUNDARYALGORITHMVTK_H
#define MAFDATABOUNDARYALGORITHMVTK_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafDataBoundaryAlgorithm.h>
#include <vtkAlgorithmOutput.h>
#include <vtkCubeSource.h>
#include <vtkTransformPolyDataFilter.h>

namespace mafPluginVTK {

// Class forwarding list

/**
Class name: mafDataBoundaryAlgorithmVTK
This class extracts the boundary from the given data value and pose matrix.
*/
class MAFPLUGINVTKSHARED_EXPORT mafDataBoundaryAlgorithmVTK : public mafResources::mafDataBoundaryAlgorithm {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafDataBoundaryAlgorithm);

public:
    /// Object constructor.
    mafDataBoundaryAlgorithmVTK(const QString code_location = "");

    /// Algorithm used to extract the boundary from the given data value and pose matrix.
    /* virtual */ mafCore::mafContainerInterface *calculateBoundary(mafCore::mafContainerInterface *data, mafResources::mafPoseMatrix *matrix = NULL);

    /// Algorithm used to extract the boundary from the given bounds and pose matrix.
    /* virtual */ mafCore::mafContainerInterface *calculateBoundary(double bounds[6], mafResources::mafPoseMatrix *matrix = NULL);

    /// Return bounds value of the bounding box.
    /* virtual */ void bounds(double bounds[6]);

protected:
    /// Object destructor.
    /* virtual */ ~mafDataBoundaryAlgorithmVTK();

private:
    mafCore::mafContainer<vtkAlgorithmOutput> m_OutputBoundary; ///< vtkAlgorithmOutput as output of the algorithm.
    vtkCubeSource *m_Box; ///< Source used as bounding box.
    vtkTransformPolyDataFilter *m_PDataFilter; ///< Filter used to transform the bounding box.
    double m_Bounds[6]; ///< Bounds of the binding box.
};


} // namespace mafPluginVTK


#endif // MAFDATABOUNDARYALGORITHMVTK_H
