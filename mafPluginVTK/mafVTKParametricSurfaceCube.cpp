/*
 *  mafVTKParametricSurfaceCube.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 19/11/11.
 *  Copyright 2011 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVTKParametricSurfaceCube.h"
#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkAlgorithmOutput.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>


using namespace mafCore;
using namespace mafPluginVTK;

mafVTKParametricSurfaceCube::mafVTKParametricSurfaceCube(const mafString code_location) : mafPluginVTK::mafVTKParametricSurface(code_location), m_CubeSource(NULL) {
    m_CubeXLength = 1.0;
    m_CubeYLength = 1.0;
    m_CubeZLength = 1.0;

    m_CubeSource = vtkCubeSource::New();
    m_Output = m_CubeSource->GetOutputPort();
    this->updateSurface();
}

mafVTKParametricSurfaceCube::~mafVTKParametricSurfaceCube(){
    m_CubeSource->Delete();
}

void mafVTKParametricSurfaceCube::updateSurface(){
    //Set parameters to surface.
    m_CubeSource->SetXLength(m_CubeXLength);
    m_CubeSource->SetYLength(m_CubeYLength);
    m_CubeSource->SetZLength(m_CubeZLength);
    m_CubeSource->SetCenter(m_Center);
    m_CubeSource->Update();
}
