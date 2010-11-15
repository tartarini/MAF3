/*
 *  mafDataBoundaryAlgorithm.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFDATABOUNDARYALGORITHM_H
#define MAFDATABOUNDARYALGORITHM_H

// Includes list
#include "mafResourcesDefinitions.h"

namespace mafResources {

// Class forwarding list

/**
Class name: mafDataBoundaryAlgorithm
This class provides basic API to extract a boundary from the given data value and pose matrix.
*/
class MAFRESOURCESSHARED_EXPORT mafDataBoundaryAlgorithm : public mafCore::mafObjectBase {
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafDataBoundaryAlgorithm(const mafString code_location = "") : mafCore::mafObjectBase(code_location) {}

    /// Algorithm that will be used to extract the boundary from the given data value and pose matrix.
    virtual mafCore::mafContainerInterface *calculateBoundary(mafCore::mafContainerInterface *data, mafPoseMatrix *matrix = NULL) = 0;

    /// Algorithm that will be used to extract the boundary from the given bounds and pose matrix.
    virtual mafCore::mafContainerInterface *calculateBoundary(double bounds[6], mafPoseMatrix *matrix = NULL) = 0;

    /// Return bounds value of the bounding box.
    virtual void bounds(double bounds[6]) = 0;
};

} // namespace mafResources


#endif // MAFDATABOUNDARYALGORITHM_H
