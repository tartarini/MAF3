/*
 *  mafDataBoundaryAlgorithm.h
 *  mafResources
 *
 *  Created by Daniele Giunchi on 30/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafDataBoundaryAlgorithm.h"

using namespace mafCore;
using namespace mafResources;

mafDataBoundaryAlgorithm::mafDataBoundaryAlgorithm(const QString code_location) : mafObjectBase(code_location) {
}


mafDataBoundaryAlgorithm::~mafDataBoundaryAlgorithm() {
}

void mafDataBoundaryAlgorithm::bounds(double bounds[6]) {
    int i = 0;
    for(i; i < 6; ++i){
        bounds[i] = m_Bounds[i];
    }
}
