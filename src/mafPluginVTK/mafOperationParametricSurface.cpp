/*
 *  mafOperationParametricSurface.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 10/02/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafOperationParametricSurface.h"
#include <mafProxy.h>
#include <vtkAlgorithmOutput.h>

using namespace mafPluginVTK;
using namespace mafResources;
using namespace mafEventBus;
using namespace mafCore;

mafOperationParametricSurface::mafOperationParametricSurface(const QString code_location) : mafOperation(code_location), m_VME(NULL), m_DataSet(NULL) {
    m_UIFilename = "mafParametricSurface.ui";
    m_ParametricSurfaceType = PARAMETRIC_SPHERE;
    m_ParametricSphere = NULL;
    m_ParametricCube = NULL;
    m_ParametricCone = NULL;
    m_ParametricCylinder = NULL;
    m_ParametricEllipsoid = NULL;
    m_ParametricSurfaceList.clear();
    m_ParametricContainer = NULL;
    this->setParametricSurfaceType(m_ParametricSurfaceType);
    this->initializeParametricSurfaces();
    this->visualizeParametricSurface();
    
    //set the icon file for vme
    
    m_VME->setProperty("iconFile",  ":/images/surface.png");
    
}

mafOperationParametricSurface::~mafOperationParametricSurface() {
    m_ParametricSurfaceList.clear();
    m_Output = NULL;
    m_ParametricContainer = NULL;
    

    mafDEL(m_ParametricSphere);
    mafDEL(m_ParametricCube);
    mafDEL(m_ParametricCone);
    mafDEL(m_ParametricCylinder);
    mafDEL(m_ParametricEllipsoid);
    mafDEL(m_DataSet);
    mafDEL(m_VME)
}

void mafOperationParametricSurface::visualizeParametricSurface() {
    mafVTKParametricSurface *currentSurface = m_ParametricSurfaceList.at(m_ParametricSurfaceType);
    m_ParametricContainer.setExternalCodecType("VTK");
    m_ParametricContainer.setClassTypeNameFunction(vtkClassTypeNameExtract);
    m_ParametricContainer = currentSurface->output();

    char *v = m_ParametricContainer.externalDataType().toAscii().data();
    
    //Insert data into VME
    m_VME = mafNEW(mafResources::mafVME);
    m_VME->setObjectName(mafTr("Parametric Surface"));
    m_DataSet = mafNEW(mafResources::mafDataSet);
    m_DataSet->setBoundaryAlgorithmName("mafPluginVTK::mafDataBoundaryAlgorithmVTK");
    m_DataSet->setDataValue(&m_ParametricContainer);
    m_VME->dataSetCollection()->insertItem(m_DataSet, 0);
    m_VME->dataSetCollection()->setPose(0., 0., 0.);
    m_VME->dataSetCollection()->setOrientation(0., 0., 0.);
    //m_VME->setProperty("visibility", true);
    this->m_Output = m_VME;

    //Notify vme add
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_VME));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);

    //Visualize vme added
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafObjectBase*, m_VME));
    argList.append(mafEventArgument(bool, true));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.sceneNodeShow", mafEventTypeLocal, &argList);
}

void mafOperationParametricSurface::initializeParametricSurfaces(){
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
    m_ParametricEllipsoid->setParent(this);
    m_ParametricSurfaceList.insert(PARAMETRIC_ELLIPSOID, m_ParametricEllipsoid);
}

bool mafOperationParametricSurface::acceptObject(mafCore::mafObjectBase *obj) {
    return obj != NULL;
}

int mafOperationParametricSurface::parametricSurfaceType() {
    return m_ParametricSurfaceType;
}

void mafOperationParametricSurface::execute() {
    m_Status = mafOperationStatusExecuting;
    Q_EMIT executionEnded();
}

void mafOperationParametricSurface::terminated() {
    if (m_Status == mafOperationStatusCanceled || m_Status == mafOperationStatusAborted) {
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafObjectBase *, m_VME));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.remove", mafEventTypeLocal, &argList);
    }
}

void mafOperationParametricSurface::unDo() {
  mafEventArgumentsList argList;
  argList.append(mafEventArgument(mafCore::mafObjectBase *, m_Output));
  mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.remove", mafEventTypeLocal, &argList);
}

void mafOperationParametricSurface::reDo() {
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_Output));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
}

void mafOperationParametricSurface::setParameters(const QVariantMap &parameters) {
    int type = parameters["parametricSurfaceType"].toInt();
    on_parametricSurfaceType_currentChanged(type);
    switch (type) {
        case (PARAMETRIC_SPHERE):
            if(parameters.contains("sphereRadius")) {
                on_sphereRadius_valueChanged(parameters["sphereRadius"].toDouble());    
            }
            
            if(parameters.contains("spherePhiRes")) {
                on_spherePhiRes_valueChanged(parameters["spherePhiRes"].toDouble());    
            }
            
            if(parameters.contains("sphereThetaRes")) {
                on_sphereThetaRes_valueChanged(parameters["sphereThetaRes"].toDouble());    
            }
            
            break;
        case (PARAMETRIC_CUBE):
            if(parameters.contains("cubeXLength")) {
                on_cubeXLength_valueChanged(parameters["cubeXLength"].toDouble());    
            }
            
            if(parameters.contains("cubeYLength")) {
                on_cubeYLength_valueChanged(parameters["cubeYLength"].toDouble());    
            }
            
            if(parameters.contains("cubeZLength")) {
                on_cubeZLength_valueChanged(parameters["cubeZLength"].toDouble());    
            }
            break;
        case (PARAMETRIC_CONE):
            if(parameters.contains("coneRadius")) {
                on_coneRadius_valueChanged(parameters["coneRadius"].toDouble());    
            }
            
            if(parameters.contains("coneHeight")) {
                on_coneHeight_valueChanged(parameters["coneHeight"].toDouble());    
            }
            
            if(parameters.contains("coneRes")) {
                on_coneRes_valueChanged(parameters["coneRes"].toDouble());    
            }
            
            if(parameters.contains("coneCapping")) {
                on_coneCapping_stateChanged(parameters["coneCapping"].toInt());    
            }
            
            break;
        case (PARAMETRIC_CYLINDER):
            if(parameters.contains("cylinderRadius")) {
                on_cylinderRadius_valueChanged(parameters["cylinderRadius"].toDouble());    
            }
            
            if(parameters.contains("cylinderHeight")) {
                on_cylinderHeight_valueChanged(parameters["cylinderHeight"].toDouble());    
            }
            
            if(parameters.contains("cylinderRes")) {
                on_cylinderRes_valueChanged(parameters["cylinderRes"].toDouble());    
            }
            break;
        case (PARAMETRIC_ELLIPSOID):
            if(parameters.contains("ellipsoidPhiRes")) {
                on_ellipsoidPhiRes_valueChanged(parameters["ellipsoidPhiRes"].toDouble());    
            }
            
            if(parameters.contains("ellipsoidThetaRes")) {
                on_ellipsoidThetaRes_valueChanged(parameters["ellipsoidThetaRes"].toDouble());    
            }
            
            if(parameters.contains("ellipsoidXLength")) {
                on_ellipsoidXLength_valueChanged(parameters["ellipsoidXLength"].toDouble());    
            }

            if(parameters.contains("ellipsoidYLength")) {
                on_ellipsoidYLength_valueChanged(parameters["ellipsoidYLength"].toDouble());    
            }
            
            if(parameters.contains("ellipsoidZLength")) {
                on_ellipsoidZLength_valueChanged(parameters["ellipsoidZLength"].toDouble());    
            }

            break;
    }
}

void mafOperationParametricSurface::setParametricSurfaceType(int index){
    m_ParametricSurfaceType = index;
}

void mafOperationParametricSurface::updateParametricSurface() {
    mafVTKParametricSurface *currentSurface = m_ParametricSurfaceList.at(m_ParametricSurfaceType);
    m_ParametricContainer = currentSurface->output();
    m_VME->dataSetCollection()->updateData();
}

void mafOperationParametricSurface::on_parametricSurfaceType_currentChanged(int index){
    this->setParametricSurfaceType(index);
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_sphereRadius_valueChanged(double d) {
    m_ParametricSphere->setProperty("sphereRadius", d);
    m_ParametricSphere->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_spherePhiRes_valueChanged(double d) {
    m_ParametricSphere->setProperty("spherePhiRes", d);
    m_ParametricSphere->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_sphereThetaRes_valueChanged(double d) {
    m_ParametricSphere->setProperty("sphereThetaRes", d);
    m_ParametricSphere->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_cubeXLength_valueChanged(double d){
    m_ParametricCube->setProperty("cubeXLength", d);
    m_ParametricCube->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_cubeYLength_valueChanged(double d) {
    m_ParametricCube->setProperty("cubeYLength", d);
    m_ParametricCube->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_cubeZLength_valueChanged(double d) {
    m_ParametricCube->setProperty("cubeZLength", d);
    m_ParametricCube->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_coneRadius_valueChanged(double d) {
    m_ParametricCone->setProperty("coneRadius", d);
    m_ParametricCone->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_coneHeight_valueChanged(double d) {
    m_ParametricCone->setProperty("coneHeight", d);
    m_ParametricCone->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_coneRes_valueChanged(double d) {
    m_ParametricCone->setProperty("coneRes", d);
    m_ParametricCone->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_coneCapping_stateChanged(int state) {
    m_ParametricCone->setProperty("coneCapping", state);
    m_ParametricCone->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_cylinderRadius_valueChanged(double d) {
    m_ParametricCylinder->setProperty("cylinderRadius", d);
    m_ParametricCylinder->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_cylinderHeight_valueChanged(double d) {
    m_ParametricCylinder->setProperty("cylinderHeight", d);
    m_ParametricCylinder->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_cylinderRes_valueChanged(double d) {
    m_ParametricCylinder->setProperty("cylinderRes", d);
    m_ParametricCylinder->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_ellipsoidPhiRes_valueChanged(double d) {
    m_ParametricEllipsoid->setProperty("ellipsoidPhiRes", d);
    m_ParametricEllipsoid->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_ellipsoidThetaRes_valueChanged(double d) {
    m_ParametricEllipsoid->setProperty("ellipsoidThetaRes", d);
    m_ParametricEllipsoid->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_ellipsoidXLength_valueChanged(double d) {
    m_ParametricEllipsoid->setProperty("ellipsoidXLength", d);
    m_ParametricEllipsoid->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_ellipsoidYLength_valueChanged(double d) {
    m_ParametricEllipsoid->setProperty("ellipsoidYLength", d);
    m_ParametricEllipsoid->updateSurface();
    this->updateParametricSurface();
}

void mafOperationParametricSurface::on_ellipsoidZLength_valueChanged(double d) {
    m_ParametricEllipsoid->setProperty("ellipsoidZLength", d);
    m_ParametricEllipsoid->updateSurface();
    this->updateParametricSurface();
}
