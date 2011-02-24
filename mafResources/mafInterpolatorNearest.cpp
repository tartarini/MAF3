/*
 *  mafInterpolatorNearest.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafInterpolatorNearest.h"

using namespace mafCore;
using namespace mafResources;

mafInterpolatorNearest::mafInterpolatorNearest(const mafString code_location) : mafInterpolator(code_location) {
}

mafInterpolatorNearest::~mafInterpolatorNearest() {
}

mafDataSet *mafInterpolatorNearest::itemAt(mafDataSetMap *collection, double t) {
    mafDataSetMap::iterator iter_low = collection->begin();
    mafDataSetMap::iterator iter_up = collection->end();
    mafDataSetMap::iterator iter_mid = iter_low;
    if(collection->size() == 0) {
        // Empty collection!!
        return NULL;
    }
    int shift_idx = collection->size() / 2;
    iter_mid += shift_idx;
    --iter_up;
    double val_low = iter_low.key();
    double value_mid = iter_mid.key();
    double value_up = iter_up.key();

    // Check if the volue is outside the timespan of the map.
    // In that case return the item value associated to one of the boundary values.
    if(t < val_low) {
        return iter_low.value();
    }
    if(t > value_up) {
        return iter_up.value();
    }

    // Execute a binary search on the items collection
    while(iter_low.key() < iter_up.key()) {
        if(t < value_mid) {
            iter_up = iter_mid;
            --iter_up;
            shift_idx /= 2;
            iter_mid -=shift_idx;
        } else if(t > value_mid) {
            iter_low = iter_mid;
            ++iter_low;
            shift_idx /= 2;
            iter_mid +=shift_idx;
        } else {
            return iter_mid.value();
        }
        value_mid = iter_mid.key();
    }
    return iter_mid.value();
}
