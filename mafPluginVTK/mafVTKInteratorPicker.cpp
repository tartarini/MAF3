/*
 *  mafVTKInteratorPicker.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 12/12/10.
 *  Copyright 2009 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVTKInteratorPicker.h"

#include <vtkSphereSource.h>
#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkTriangleFilter.h>
#include <vtkCylinderSource.h>
#include <vtkPlaneSource.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransform.h>
#include <vtkAlgorithmOutput.h>
#include <vtkSmartPointer.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafPluginVTK;

mafVTKInteratorPicker::mafVTKInteratorPicker(const mafString code_location) : mafObjectBase(code_location), m_SurfaceOutput(NULL) {
    m_GeometryType = 0;
    m_SphereRadius = 2.0;
    m_SpherePhiRes = 10.0;
    m_SphereTheRes = 10.0;
    m_ConeHeight = 5.0;
    m_ConeRadius = 2.0;
    m_ConeCapping = 0;
    m_ConeRes = 20.0;
    m_ConeOrientationAxis = ID_X_AXIS;
    m_CylinderHeight = 5.0;
    m_CylinderRadius = 2.0;
    m_CylinderRes = 20.0;
    m_CylinderOrientationAxis = ID_Y_AXIS;
    m_CubeXLength = 2.0;
    m_CubeYLength = 2.0;
    m_CubeZLength = 2.0;
    m_Center[0] = m_Center[1] = m_Center[2] = 0;
    m_EllipsoidXLenght = 1.0;
    m_EllipsoidYLenght = 2.0;
    m_EllipsoidZLenght = 3.0;
    m_EllipsoidPhiRes = 10.0;
    m_EllipsoidTheRes = 10.0;
    m_EllipsoidOrientationAxis = ID_X_AXIS;
}

void mafVTKInteratorPicker::initializeConnections() {

    mafId vme_picked_id = mafIdProvider::instance()->idValue("maf.local.resources.interaction.vmePick");
    if(vme_picked_id == -1) {
        mafIdProvider::instance()->createNewId("maf.local.resources.interaction.vmePick");

        // Register API signals.
        mafRegisterLocalSignal("maf.local.resources.interaction.vmePick", this, "vmePickSignal(double *, unsigned long, mafCore::mafContainerInterface *, QEvent *)");

        // Register private callbacks.
        mafRegisterLocalCallback("maf.local.resources.interaction.vmePick", this, "vmePick(double *, unsigned long, mafCore::mafContainerInterface *, QEvent *)");
    }
}

void mafVTKInteratorPicker::vmePick(double *pickPos, unsigned long modifiers, mafCore::mafContainerInterface *actor, QEvent *e ) {
    //Create a surface on the picking position
    m_Center = pickPos;
    this->internalUpdate();
}

void mafVTKInteratorPicker::setGeometryType(int parametricSurfaceTypeID) {
    m_GeometryType = parametricSurfaceTypeID;
}

void mafVTKInteratorPicker::internalUpdate()
{
    switch(m_GeometryType)
    {
    case PARAMETRIC_SPHERE:
        {
            vtkSmartPointer<vtkSphereSource> surf;
            surf->SetRadius(m_SphereRadius);
            surf->SetPhiResolution(m_SpherePhiRes);
            surf->SetThetaResolution(m_SphereTheRes);
            surf->SetCenter(m_Center);
            surf->Update();
            m_SurfaceOutput = surf->GetOutputPort();
        }
        break;
    case PARAMETRIC_CONE:
        {
            vtkSmartPointer<vtkConeSource> surf;
            surf->SetHeight(m_ConeHeight);
            surf->SetRadius(m_ConeRadius);
            surf->SetCapping(m_ConeCapping);
            surf->SetResolution(m_ConeRes);
            surf->SetCenter(m_Center);
            surf->Update();
            vtkSmartPointer<vtkTransform> t;
            switch(m_ConeOrientationAxis)
            {
            case ID_X_AXIS:
                //do nothing
                break;
            case ID_Y_AXIS:
                t->RotateZ(90);
                break;
            case ID_Z_AXIS:
                t->RotateY(-90);
                break;
            default:
                break;
            }
            t->Update();
            vtkSmartPointer<vtkTransformPolyDataFilter> ptf;
            ptf->SetTransform(t);
            ptf->SetInput(surf->GetOutput());
            ptf->Update();
            m_SurfaceOutput = ptf->GetOutputPort();
        }
        break;
    case PARAMETRIC_CYLINDER:
        {
            vtkSmartPointer<vtkCylinderSource> surf;
            surf->SetHeight(m_CylinderHeight);
            surf->SetRadius(m_CylinderRadius);
            surf->SetResolution(m_CylinderRes);
            surf->SetCenter(m_Center);
            surf->Update();
            vtkSmartPointer<vtkTransform> t;
            switch(m_CylinderOrientationAxis)
            {
            case ID_X_AXIS:
                t->RotateZ(90);
                break;
            case ID_Y_AXIS:
                //do nothing
                break;
            case ID_Z_AXIS:
                t->RotateX(-90);
                break;
            default:
                break;
            }
            t->Update();
            vtkSmartPointer<vtkTransformPolyDataFilter> ptf;
            ptf->SetTransform(t);
            ptf->SetInput(surf->GetOutput());
            ptf->Update();
            m_SurfaceOutput = ptf->GetOutputPort();
        }
        break;
    case PARAMETRIC_CUBE:
        {
            vtkSmartPointer<vtkCubeSource> surf;
            surf->SetXLength(m_CubeXLength);
            surf->SetYLength(m_CubeYLength);
            surf->SetZLength(m_CubeZLength);
            surf->SetCenter(m_Center);
            surf->Update();
            m_SurfaceOutput = surf->GetOutputPort();
        }
        break;
    case PARAMETRIC_ELLIPSOID:
        {
            vtkSmartPointer<vtkSphereSource> surf;
            surf->SetRadius(m_EllipsoidYLenght);
            surf->SetPhiResolution(m_EllipsoidPhiRes);
            surf->SetThetaResolution(m_EllipsoidTheRes);
            surf->SetCenter(m_Center);
            surf->Update();
            vtkSmartPointer<vtkTransform> t;
            switch(m_EllipsoidOrientationAxis)
            {
            case ID_X_AXIS:
                //do nothing
                break;
            case ID_Y_AXIS:
                t->RotateZ(90);
                break;
            case ID_Z_AXIS:
                t->RotateY(-90);
                break;
            default:
                break;
            }

            t->Scale(m_EllipsoidXLenght/m_EllipsoidYLenght,1,m_EllipsoidZLenght/m_EllipsoidYLenght);
            t->Update();
            vtkSmartPointer<vtkTransformPolyDataFilter> ptf;
            ptf->SetTransform(t);
            ptf->SetInput(surf->GetOutput());
            ptf->Update();
            m_SurfaceOutput = ptf->GetOutputPort();
        }
        break;
    }
}

void mafVTKInteratorPicker::setSphereRadius(double sphereRadius){
    m_SphereRadius = sphereRadius;
}

void mafVTKInteratorPicker::setSpherePhiResolution(double spherePhiRes){
    m_SpherePhiRes = spherePhiRes;
}

void mafVTKInteratorPicker::setThetaResolution(double sphereTheRes){
    m_SphereTheRes = sphereTheRes;
}

void mafVTKInteratorPicker::setConeHeight(double coneHeight){
    m_ConeHeight = coneHeight;
}

void mafVTKInteratorPicker::setConeRadius(double coneRadius){
    m_ConeRadius = coneRadius;
}

void mafVTKInteratorPicker::setConeCapping(int coneCapping){
    m_ConeCapping = coneCapping;
}

void mafVTKInteratorPicker::setConeResolution(double coneRes){
    m_ConeRes = coneRes;
}

void mafVTKInteratorPicker::setCylinderHeight(double cylinderHeight){
    m_CylinderHeight = cylinderHeight;
}

void mafVTKInteratorPicker::setCylinderRadius(double cylinderRadius){
    m_CylinderRadius = cylinderRadius;
}

void mafVTKInteratorPicker::setCylinderResolution(double cylinderRes){
    m_CylinderRes = cylinderRes;
}

void mafVTKInteratorPicker::setCubeSide(double XLength, double YLength, double ZLength){
    m_CubeXLength = XLength;
    m_CubeYLength = YLength;
    m_CubeZLength = ZLength;
}

void mafVTKInteratorPicker::setEllipsoidRadius(double ellipsoidYLenght){
    m_EllipsoidYLenght = ellipsoidYLenght;
}

void mafVTKInteratorPicker::setEllipsoidPhiResolution(double ellipsoidPhiRes){
    m_EllipsoidPhiRes = ellipsoidPhiRes;
}

void mafVTKInteratorPicker::setEllipsoidThetaResolution(double ellipsoidTheRes){
    m_EllipsoidTheRes = ellipsoidTheRes;
}
