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
    m_UIFilename = "mafOpParametricSurface.ui";
    m_ParametricSurfaceType = PARAMETRIC_SPHERE;
    m_ParametricSphere = NULL;
    m_ParametricCube = NULL;
    m_ParametricCone = NULL;
    m_ParametricCylinder = NULL;
    m_ParametricEllipsoid = NULL;
    m_ParametricSurfaceList.clear();
    m_ParametricContainer = NULL;
    this->setParametricSurfaceType(m_ParametricSurfaceType);
    initializeParametricSurfaces();
}

mafOpParametricSurface::~mafOpParametricSurface() {
    m_ParametricSurfaceList.clear();
    m_Output = NULL;
    m_ParametricContainer = NULL;
    mafDEL(m_DataSet);
    mafDEL(m_VME);

    //removed temporarly
    mafDEL(m_ParametricSphere);
    mafDEL(m_ParametricCube);
    mafDEL(m_ParametricCone);
    mafDEL(m_ParametricCylinder);
    mafDEL(m_ParametricEllipsoid);
}

void mafOpParametricSurface::initializeParametricSurfaces(){
    m_ParametricSphere = (mafVTKParametricSurfaceSphere *)mafNEWFromString("mafPluginVTK::mafVTKParametricSurfaceSphere");
    m_ParametricSphere->setParent(this);
    m_ParametricSurfaceList.insert(PARAMETRIC_SPHERE, m_ParametricSphere);

    m_ParametricCube = (mafVTKParametricSurfaceCube *)mafNEWFromString("mafPluginVTK::mafVTKParametricSurfaceCube");
    m_ParametricCube->setParent(this);
    m_ParametricSurfaceList.insert(PARAMETRIC_CUBE, m_ParametricCube);

    m_ParametricCone = (mafVTKParametricSurfaceCone *)mafNEWFromString("mafPluginVTK::mafVTKParametricSurfaceCone");
    m_ParametricCone->setParent(this);
    m_ParametricSurfaceList.insert(PARAMETRIC_CONE, m_ParametricCone);

    m_ParametricCylinder = (mafVTKParametricSurfaceCylinder *)mafNEWFromString("mafPluginVTK::mafVTKParametricSurfaceCylinder");
    m_ParametricCylinder->setParent(this);
    m_ParametricSurfaceList.insert(PARAMETRIC_CYLINDER, m_ParametricCylinder);

    m_ParametricEllipsoid = (mafVTKParametricSurfaceEllipsoid *)mafNEWFromString("mafPluginVTK::mafVTKParametricSurfaceEllipsoid");
    m_ParametricCylinder->setParent(this);
    m_ParametricSurfaceList.insert(PARAMETRIC_ELLIPSOID, m_ParametricEllipsoid);
}

bool mafOpParametricSurface::acceptObject(mafCore::mafObjectBase *obj) {
    if(obj == NULL) {
        return false;
    }
    mafString ct(obj->metaObject()->className());
    return ct == "mafResources::mafVME";
}

int mafOpParametricSurface::parametricSurfaceType() {
    return m_ParametricSurfaceType;
}

void mafOpParametricSurface::execute() {
    mafVTKParametricSurface *currentSurface = m_ParametricSurfaceList.at(m_ParametricSurfaceType);
    m_ParametricContainer = currentSurface->output();

    //Insert data into VME
    m_VME = mafNEW(mafResources::mafVME);
    m_VME->setObjectName(mafTr("Parametric Surface"));
    m_DataSet = mafNEW(mafResources::mafDataSet);
    m_DataSet->setDataValue(&m_ParametricContainer);
    m_VME->dataSetCollection()->insertItem(m_DataSet, 0);
    this->m_Output = m_VME;

    //Notify vme add
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_VME));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
}

void mafOpParametricSurface::setParameters(mafList<mafVariant> parameters) {
    Q_UNUSED(parameters);
}

void mafOpParametricSurface::setParametricSurfaceType(int index){
    m_ParametricSurfaceType = index;
}


void mafOpParametricSurface::on_parametricSurfaceType_currentChanged(int index){
    this->setParametricSurfaceType(index);
}

void mafOpParametricSurface::on_sphereRadius_valueChanged(double d) {
    m_ParametricSphere->setProperty("sphereRadius", d);
    m_ParametricSphere->updateSurface();
}

void mafOpParametricSurface::on_spherePhiRes_valueChanged(double d) {
    m_ParametricSphere->setProperty("spherePhiRes", d);
    m_ParametricSphere->updateSurface();
}

void mafOpParametricSurface::on_sphereThetaRes_valueChanged(double d) {
    m_ParametricSphere->setProperty("sphereTheRes", d);
    m_ParametricSphere->updateSurface();
}

void mafOpParametricSurface::on_cubeXLength_valueChanged(double d){
    m_ParametricCube->setProperty("cubeXLength", d);
    m_ParametricCube->updateSurface();
}

void mafOpParametricSurface::on_cubeYLength_valueChanged(double d) {
    m_ParametricCube->setProperty("cubeYLength", d);
    m_ParametricCube->updateSurface();
}

void mafOpParametricSurface::on_cubeZLength_valueChanged(double d) {
    m_ParametricCube->setProperty("cubeZLength", d);
    m_ParametricCube->updateSurface();
}

void mafOpParametricSurface::on_coneRadius_valueChanged(double d) {
    m_ParametricCone->setProperty("coneRadius", d);
    m_ParametricCone->updateSurface();
}

void mafOpParametricSurface::on_coneHeight_valueChanged(double d) {
    m_ParametricCone->setProperty("coneHeight", d);
    m_ParametricCone->updateSurface();
}

void mafOpParametricSurface::on_coneRes_valueChanged(double d) {
    m_ParametricCone->setProperty("coneRes", d);
    m_ParametricCone->updateSurface();
}

void mafOpParametricSurface::on_coneCapping_stateChanged(int state) {
    m_ParametricCone->setProperty("coneCap", state);
    m_ParametricCone->updateSurface();
}

void mafOpParametricSurface::on_cylinderRadius_valueChanged(double d) {
    m_ParametricCylinder->setProperty("cylinderRadius", d);
    m_ParametricCylinder->updateSurface();
}

void mafOpParametricSurface::on_cylinderHeight_valueChanged(double d) {
    m_ParametricCylinder->setProperty("cylinderHeight", d);
    m_ParametricCylinder->updateSurface();
}

void mafOpParametricSurface::on_cylinderRes_valueChanged(double d) {
    m_ParametricCylinder->setProperty("cylinderRes", d);
    m_ParametricCylinder->updateSurface();
}

void mafOpParametricSurface::on_ellipsoidRadius_valueChanged(double d){
    m_ParametricEllipsoid->setProperty("ellipsoidRadius", d);
    m_ParametricEllipsoid->updateSurface();
}

void mafOpParametricSurface::on_ellipsoidPhiRes_valueChanged(double d) {
    m_ParametricEllipsoid->setProperty("ellipsoidPhiRes", d);
    m_ParametricEllipsoid->updateSurface();
}

void mafOpParametricSurface::on_ellipsoidThetaRes_valueChanged(double d) {
    m_ParametricEllipsoid->setProperty("ellipsoidThetaRes", d);
    m_ParametricEllipsoid->updateSurface();
}

void mafOpParametricSurface::on_ellipsoidXLength_valueChanged(double d) {
    m_ParametricEllipsoid->setProperty("ellipsoidXLength", d);
    m_ParametricEllipsoid->updateSurface();
}

void mafOpParametricSurface::on_ellipsoidYLength_valueChanged(double d) {
    m_ParametricEllipsoid->setProperty("ellipsoidYLength", d);
    m_ParametricEllipsoid->updateSurface();
}

void mafOpParametricSurface::on_ellipsoidZLength_valueChanged(double d) {
    m_ParametricEllipsoid->setProperty("ellipsoidZLength", d);
    m_ParametricEllipsoid->updateSurface();
}


