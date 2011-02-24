/*
 *  mafInterpolatorBefore.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafInterpolatorBefore.h"

using namespace mafCore;
using namespace mafResources;

mafInterpolatorBefore::mafInterpolatorBefore(const mafString code_location) : mafInterpolator(code_location) {
}

mafInterpolatorBefore::~mafInterpolatorBefore() {
}

mafDataSet *mafInterpolatorBefore::itemAt(mafDataSetMap *collection, double t) {
    mafDataSetMap::const_iterator iter = collection->constBegin();
    while(iter != collection->constEnd() && iter.key() <= t) {
        ++iter;
    }
    // The iterator has to be different from the begin and the end of the iteration because the interpolation
    // mechanism is to return the item before the cuttent given timestamp.
    if(iter != collection->constBegin()) {
        --iter;
        return iter.value();
    }
    return NULL;
}
