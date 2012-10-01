/*
 *  mafDataBoundaryAlgorithm.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
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
    mafDataBoundaryAlgorithm(const QString code_location = "");

    /// Algorithm that will be used to extract the boundary from the given data value and pose matrix.
    virtual mafCore::mafProxyInterface *calculateBoundary(mafCore::mafProxyInterface *data, mafMatrix *matrix = NULL) = 0;

    /// Return bounds value of the bounding box.
    virtual void bounds(double bounds[6]);

protected:
    /// Algorithm that will be used to extract the boundary from the given bounds and pose matrix.
    virtual mafCore::mafProxyInterface *calculateBoundary(double bounds[6], mafMatrix *matrix = NULL) = 0;
    
    /// Object destructor.
    /* virtual */ ~mafDataBoundaryAlgorithm();

     double m_Bounds[6]; ///< Bounds of the bounding box.
};

} // namespace mafResources


#endif // MAFDATABOUNDARYALGORITHM_H
