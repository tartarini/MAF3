/*
 *  mafDataBoundaryAlgorithm.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 10/11/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
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
    /* virtual */ mafCore::mafProxyInterface *calculateBoundary(mafCore::mafProxyInterface *data, mafResources::mafMatrix *matrix = NULL);

protected:
    /// Algorithm used to extract the boundary from the given bounds and pose matrix.
    /* virtual */ mafCore::mafProxyInterface *calculateBoundary(double bounds[6], mafResources::mafMatrix *matrix = NULL);
    
    /// Object destructor.
    /* virtual */ ~mafDataBoundaryAlgorithmVTK();

private:
    mafCore::mafProxy<vtkAlgorithmOutput> m_OutputBoundary; ///< vtkAlgorithmOutput as output of the algorithm.
    vtkCubeSource *m_Box; ///< Source used as bounding box.
    vtkTransformPolyDataFilter *m_PDataFilter; ///< Filter used to transform the bounding box.
};


} // namespace mafPluginVTK


#endif // MAFDATABOUNDARYALGORITHMVTK_H
