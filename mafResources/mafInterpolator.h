/*
 *  mafInterpolator.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFINTERPOLATOR_H
#define MAFINTERPOLATOR_H

// Includes list
#include "mafResourcesDefinitions.h"

namespace mafResources {

// Class forwarding list

/**
Class name: mafInterpolator
This class provides basic API to search elements inside a container with a defined search approaches.
If no element is found NULL is returned.
*/
class MAFRESOURCESSHARED_EXPORT mafInterpolator : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafInterpolator(const mafString code_location = "");

    /// Search the item at the given timestamp 't' with the defined interpolation strategy.
    virtual mafDataSet *itemAt(mafDataSetMap *collection, double t) = 0;
};

} // namespace mafResources

#endif // MAFINTERPOLATOR_H
