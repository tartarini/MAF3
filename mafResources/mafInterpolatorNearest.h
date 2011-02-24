/*
 *  mafInterpolatorNearest.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFINTERPOLATORNEAREST_H
#define MAFINTERPOLATORNEAREST_H

// Includes list
#include "mafInterpolator.h"

namespace mafResources {

// Class forwarding list

/**
Class name: mafInterpolatorNearest
This class provides the mechanism to return the element nearest the the given timestamp.
First element or last one are returned for timestamps outside the definition range of the collection.
*/
class MAFRESOURCESSHARED_EXPORT mafInterpolatorNearest : public mafInterpolator {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafInterpolator);

public:
    /// Object constructor.
    mafInterpolatorNearest(const mafString code_location = "");

    /// Search the item at the given timestamp 't' with the defined interpolation strategy.
    /*virtual*/ mafDataSet *itemAt(mafDataSetMap *collection, double t);

protected:
    /// Object destructor.
    /* virtual */ ~mafInterpolatorNearest();
};

} //mafResources

#endif // MAFINTERPOLATORNEAREST_H
