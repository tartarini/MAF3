/*
 *  mafInterpolatorBefore.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFINTERPOLATORBEFORE_H
#define MAFINTERPOLATORBEFORE_H

// Includes list
#include "mafInterpolator.h"

namespace mafResources {

// Class forwarding list

/**
Class name: mafInterpolatorBefore
This class provides the mechanism to return the element suddenly before the the given timestamp.
If no elements are defined befor the given timestamp, NULL is returned.
*/
class MAFRESOURCESSHARED_EXPORT mafInterpolatorBefore : public mafInterpolator {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafInterpolator);

public:
    /// Object constructor.
    mafInterpolatorBefore(const QString code_location = "");

    /// Search the item at the given timestamp 't' with the defined interpolation strategy.
    /*virtual*/ mafDataSet *itemAt(mafDataSetMap *collection, double t);

protected:
    /// Object destructor.
    /* virtual */  ~mafInterpolatorBefore();
};

} //mafResources

#endif // MAFINTERPOLATORBEFORE_H
