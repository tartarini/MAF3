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
    mafDataBoundaryAlgorithmVTK(const mafString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafDataBoundaryAlgorithmVTK();

    /// Algorithm used to extract the boundary from the given data value and pose matrix.
    mafCore::mafContainerInterface *calculateBoundary(mafCore::mafContainerInterface *data, mafResources::mafPoseMatrix *matrix);
};

} // namespace mafPluginVTK


#endif // MAFDATABOUNDARYALGORITHMVTK_H
