/*
 *  mafOpParametricSurface.cpp
 *  SimpleApp
 *
 *  Created by Roberto Mucci on 10/02/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafOpParametricSurface.h"
#include <mafContainer.h>
#include <mafContainerInterface.h>
#include <vtkAlgorithmOutput.h>

using namespace mafPluginVTK;
using namespace mafResources;
using namespace mafEventBus;
using namespace mafCore;

mafOpParametricSurface::mafOpParametricSurface(const mafString code_location) : mafOperation(code_location), m_VME(NULL), m_DataSet(NULL) {
    //m_UIFilename = "mafOpParametricSurface.ui";
    m_ParametricSurfaceType = PARAMETRIC_SPHERE;
    m_ParametricSphere = NULL;
    m_ParametricCube = NULL;
    m_ParametricCone = NULL;
    m_ParametricCylinder = NULL;
    m_ParametricEllipsoid = NULL;
    m_ParametricSurfaceList.clear();
    this->setSurfaceType(m_ParametricSurfaceType);
}

mafOpParametricSurface::~mafOpParametricSurface() {
    mafDEL(m_DataSet);
    mafDEL(m_VME);
    m_ParametricSurfaceList.clear();
    mafDEL(m_ParametricSphere);
    mafDEL(m_ParametricCube);
    mafDEL(m_ParametricCone);
    mafDEL(m_ParametricCylinder);
    mafDEL(m_ParametricEllipsoid);
    m_Output = NULL;
}

bool mafOpParametricSurface::acceptObject(mafCore::mafObjectBase *obj) {
    if(obj == NULL) {
        return false;
    }
    mafString ct(obj->metaObject()->className());
    return ct == "mafResources::mafVME";
}
void mafOpParametricSurface::setSurfaceType(int parametricSurfaceType){
     //Creates different instances of parametric surface, in order to store parameters
    switch (parametricSurfaceType){
        case PARAMETRIC_SPHERE:
            if (m_ParametricSphere == NULL){
                m_ParametricSphere = (mafVTKParametricSurfaceSphere *)mafNEWFromString("mafPluginVTK::mafVTKParametricSurfaceSphere");
                m_ParametricSurfaceList.insert(parametricSurfaceType, m_ParametricSphere);
            }
            break;
       case PARAMETRIC_CUBE:
            if (m_ParametricCube == NULL){
                m_ParametricCube = (mafVTKParametricSurfaceCube *)mafNEWFromString("mafPluginVTK::mafVTKParametricSurfaceCube");
                m_ParametricSurfaceList.insert(parametricSurfaceType, m_ParametricCube);
            }
            break;
        case PARAMETRIC_CONE:
            if (m_ParametricCone == NULL){
                m_ParametricCone = (mafVTKParametricSurfaceCone *)mafNEWFromString("mafPluginVTK::mafVTKParametricSurfaceCone");
                m_ParametricSurfaceList.insert(parametricSurfaceType, m_ParametricCone);
            }
            break;
        case PARAMETRIC_CYLINDER:
            if (m_ParametricCylinder == NULL){
                m_ParametricCylinder = (mafVTKParametricSurfaceCylinder *)mafNEWFromString("mafPluginVTK::mafVTKParametricSurfaceCylinder");
                m_ParametricSurfaceList.insert(parametricSurfaceType, m_ParametricCylinder);
            }
            break;
        case PARAMETRIC_ELLIPSOID:
            if (m_ParametricEllipsoid == NULL){
                m_ParametricEllipsoid = (mafVTKParametricSurfaceEllipsoid *)mafNEWFromString("mafPluginVTK::mafVTKParametricSurfaceEllipsoid");
                m_ParametricSurfaceList.insert(parametricSurfaceType, m_ParametricEllipsoid);
            }
            break;
    }
}

void mafOpParametricSurface::execute() {
    mafVTKParametricSurface *currentSurface = m_ParametricSurfaceList.at(m_ParametricSurfaceType);
    m_ParametricContainer = currentSurface->output();

    //Insert data into VME
    m_VME = mafNEW(mafResources::mafVME);
    m_DataSet = mafNEW(mafResources::mafDataSet);
    m_DataSet->setDataValue(&m_ParametricContainer);
    m_VME->dataSetCollection()->insertItem(m_DataSet, 0);
    this->m_Output = m_VME;

    //Notify vme addd
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_VME));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
}

void mafOpParametricSurface::setParameters(mafList<mafVariant> parameters) {
    Q_UNUSED(parameters);
}


