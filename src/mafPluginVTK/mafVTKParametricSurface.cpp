/*
 *  mafVTKParametricSurface.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 19/11/11.
 *  Copyright 2009 SCS-B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVTKParametricSurface.h"

using namespace mafCore;
using namespace mafPluginVTK;


mafVTKParametricSurface::mafVTKParametricSurface(const QString code_location) : mafCore::mafObject(code_location), m_Output(NULL) {
    m_Center[0] = 0;
    m_Center[1] = 0;
    m_Center[2] = 0;
}

mafVTKParametricSurface::~mafVTKParametricSurface() {
}

vtkAlgorithmOutput* mafVTKParametricSurface::output(){
    return m_Output;
}

void mafVTKParametricSurface::updateSurface(){
}


