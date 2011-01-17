/*
 *  mafVTKInteractorPicker.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 12/12/10.
 *  Copyright 2009 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVTKInteractorPicker.h"

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
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkAppendPolyData.h>
#include <vtkCellLocator.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>

#define FIXED_MARKER_COLOR  0
#define TMP_MARKER_COLOR    1

using namespace mafCore;
using namespace mafEventBus;
using namespace mafPluginVTK;

mafVTKInteractorPicker::mafVTKInteractorPicker(const mafString code_location) : mafResources::mafInteractor(code_location), m_MarkerOutput(NULL), m_TmpMarkerOutput(NULL), m_LastMarkerOutput(NULL), m_Mapper(NULL), m_Output(NULL) {
    m_MarkerIndex = 0;
    m_GeometryType = 0;
    m_SphereRadius = 1.0;
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
    m_Center = NULL;
    m_EllipsoidXLenght = 1.0;
    m_EllipsoidYLenght = 2.0;
    m_EllipsoidZLenght = 3.0;
    m_EllipsoidPhiRes = 10.0;
    m_EllipsoidTheRes = 10.0;
    m_EllipsoidOrientationAxis = ID_X_AXIS;
    m_IsFirstPick = true;
    m_PointList.clear();
    this->initializeConnections();
}

void mafVTKInteractorPicker::initializeConnections() {
    this->createPipe();

    mafId vme_picked_id = mafIdProvider::instance()->idValue("maf.local.resources.interaction.vmePick");
    //if(vme_picked_id == -1) {
        mafIdProvider::instance()->createNewId("maf.local.resources.interaction.vmePick");

        // Register API signals.
        mafRegisterLocalSignal("maf.local.resources.interaction.vmePick", this, "vmePickSignal(double *, unsigned long, mafCore::mafContainerInterface *, QEvent *)");

        // Register private callbacks.
        mafRegisterLocalCallback("maf.local.resources.interaction.vmePick", this, "vmePick(double *, unsigned long, mafCore::mafContainerInterface *, QEvent *)");
    //}

    // Register API signals.
    mafRegisterLocalSignal("maf.local.operation.VTK.nextPick", this, "nextPickSignal()");

    // Register private callbacks.
    mafRegisterLocalCallback("maf.local.operation.VTK.nextPick", this, "nextPick()");

    // Register API signals.
    //mafRegisterLocalSignal("maf.local.operation.VTK.OK", this, "OKSignal()");

    // Register private callbacks.
    mafRegisterLocalCallback("maf.local.operation.VTK.OK", this, "OK()");

    // Register API signals.
    //mafRegisterLocalSignal("maf.local.operation.VTK.unDoPick", this, "unDoPickSignal()");

    // Register private callbacks.
    //mafRegisterLocalCallback("maf.local.operation.VTK.unDoPick", this, "unDoPick()");
}

void mafVTKInteractorPicker::createPipe() {
    m_Mapper = vtkPolyDataMapper::New();
    m_Mapper->SetScalarRange(FIXED_MARKER_COLOR, TMP_MARKER_COLOR);
    m_Actor = vtkActor::New();
    m_Actor.setDestructionFunction(&vtkActor::Delete);
    m_Actor->SetMapper(m_Mapper);
    m_Output = &m_Actor;
    m_AppendData = vtkAppendPolyData::New();
    m_Mapper->SetInputConnection(m_AppendData->GetOutputPort());
}

void mafVTKInteractorPicker::vmePick(double *pickPos, unsigned long modifiers, mafCore::mafContainerInterface *actor, QEvent *e ) {
    //Check if ctrl is pressed
    if((modifiers&(1<<MAF_CTRL_KEY))!=0){
        //remove the nearest marker
        double closestPoint[3]; //the coordinates of the closest point will be returned here
        double closestPointDist2; //the squared distance to the closest point will be returned here
        double tmptClosestPointDist2;
        vtkIdType cellId;
        int subId;

        int InputNumber = m_AppendData->GetNumberOfInputConnections(0);
        int closestMarkerIndex = 0;
        int i = 0;
        for(i; i < InputNumber; i++){
            vtkSmartPointer<vtkCellLocator> cellLocator = vtkSmartPointer<vtkCellLocator>::New();
            cellLocator->SetDataSet(m_AppendData->GetInput(i));
            cellLocator->BuildLocator();
            cellLocator->FindClosestPoint(pickPos, closestPoint, cellId, subId, closestPointDist2);
            if (i == 0) {
                tmptClosestPointDist2 = closestPointDist2;
            } else if (closestPointDist2 < tmptClosestPointDist2){
                tmptClosestPointDist2 = closestPointDist2;
                closestMarkerIndex = i;
            }
        }
        m_AppendData->RemoveInputConnection(0,m_AppendData->GetInputConnection(0,closestMarkerIndex));
        m_PointList.removeAt(closestMarkerIndex);

        if (closestMarkerIndex == InputNumber-1){
            //then last marker has been deleted
            m_IsFirstPick = true;
        } else {
            m_MarkerIndex--;
        }

    } else {
        if(!m_IsFirstPick) {
            //Remove last pick surface
            m_AppendData->RemoveInputConnection(0, m_TmpMarkerOutput);
        }

        //Create a surface on the picking position
        m_Center = pickPos;
        this->internalUpdate();

        //Show current pick surface
        m_TmpMarkerOutput = m_MarkerOutput;
        m_IsFirstPick = false;
    }
}

void mafVTKInteractorPicker::nextPick() {
    //Set last pick position as final
    m_PointList.append(m_Center);
    vtkPolyData *data = m_AppendData->GetInput(m_MarkerIndex);
    this->setScalarValue(data, FIXED_MARKER_COLOR);

    m_AppendData->Modified(); //to update scalar of input polydata
    m_LastMarkerOutput = m_MarkerOutput;
    m_IsFirstPick = true;
    m_MarkerIndex++;
}

void mafVTKInteractorPicker::OK() {
    //Set last pick position as final
    m_PointList.append(m_Center);

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafList<double *>, m_PointList));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.pickedList", mafEventTypeLocal, &argList);
}

/*void mafVTKInteractorPicker::unDoPick() {
    //Set last pick position as final
    m_PointList.removeLast();

    if(!m_IsFirstPick) {
        //Remove last pick surface
        m_AppendData->RemoveInputConnection(0, m_TmpMarkerOutput);
    }

    m_AppendData->RemoveInputConnection(0, m_LastMarkerOutput);

    //Remove last pick surface
    m_IsFirstPick = true;
}*/

void mafVTKInteractorPicker::setGeometryType(int parametricSurfaceTypeID) {
    m_GeometryType = parametricSurfaceTypeID;
}

void mafVTKInteractorPicker::internalUpdate()
{
    switch(m_GeometryType)
    {
    case PARAMETRIC_SPHERE:
        {
            vtkSmartPointer<vtkSphereSource> surf = vtkSmartPointer<vtkSphereSource>::New();
            surf->SetRadius(m_SphereRadius);
            surf->SetPhiResolution(m_SpherePhiRes);
            surf->SetThetaResolution(m_SphereTheRes);
            surf->SetCenter(m_Center);
            surf->Update();

            this->setScalarValue(surf->GetOutput(), TMP_MARKER_COLOR);
            m_MarkerOutput = surf->GetOutputPort();
            m_AppendData->AddInputConnection(m_MarkerOutput);
        }
        break;
    case PARAMETRIC_CONE:
        {
            vtkSmartPointer<vtkConeSource> surf = vtkSmartPointer<vtkConeSource>::New();
            surf->SetHeight(m_ConeHeight);
            surf->SetRadius(m_ConeRadius);
            surf->SetCapping(m_ConeCapping);
            surf->SetResolution(m_ConeRes);
            surf->SetCenter(m_Center);
            surf->Update();
            this->setScalarValue(surf->GetOutput(), TMP_MARKER_COLOR);
            vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
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
            vtkSmartPointer<vtkTransformPolyDataFilter> ptf = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
            ptf->SetTransform(t);
            ptf->SetInput(surf->GetOutput());
            ptf->Update();
            m_MarkerOutput = ptf->GetOutputPort();
            m_AppendData->AddInputConnection(m_MarkerOutput);
        }
        break;
    case PARAMETRIC_CYLINDER:
        {
            vtkSmartPointer<vtkCylinderSource> surf = vtkSmartPointer<vtkCylinderSource>::New();
            surf->SetHeight(m_CylinderHeight);
            surf->SetRadius(m_CylinderRadius);
            surf->SetResolution(m_CylinderRes);
            surf->SetCenter(m_Center);
            surf->Update();
            this->setScalarValue(surf->GetOutput(), TMP_MARKER_COLOR);
            vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
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
            vtkSmartPointer<vtkTransformPolyDataFilter> ptf = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
            ptf->SetTransform(t);
            ptf->SetInput(surf->GetOutput());
            ptf->Update();
            m_MarkerOutput = ptf->GetOutputPort();
            m_AppendData->AddInputConnection(m_MarkerOutput);
        }
        break;
    case PARAMETRIC_CUBE:
        {
            vtkSmartPointer<vtkCubeSource> surf = vtkSmartPointer<vtkCubeSource>::New();
            surf->SetXLength(m_CubeXLength);
            surf->SetYLength(m_CubeYLength);
            surf->SetZLength(m_CubeZLength);
            surf->SetCenter(m_Center);
            surf->Update();
            this->setScalarValue(surf->GetOutput(), TMP_MARKER_COLOR);
            m_MarkerOutput = surf->GetOutputPort();
            m_AppendData->AddInputConnection(m_MarkerOutput);
        }
        break;
    case PARAMETRIC_ELLIPSOID:
        {
            vtkSmartPointer<vtkSphereSource> surf = vtkSmartPointer<vtkSphereSource>::New();
            surf->SetRadius(m_EllipsoidYLenght);
            surf->SetPhiResolution(m_EllipsoidPhiRes);
            surf->SetThetaResolution(m_EllipsoidTheRes);
            surf->SetCenter(m_Center);
            surf->Update();
            this->setScalarValue(surf->GetOutput(), TMP_MARKER_COLOR);
            vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
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
            vtkSmartPointer<vtkTransformPolyDataFilter> ptf = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
            ptf->SetTransform(t);
            ptf->SetInput(surf->GetOutput());
            ptf->Update();
            m_MarkerOutput = ptf->GetOutputPort();
            m_AppendData->AddInputConnection(m_MarkerOutput);
        }
        break;
    }
}

void mafVTKInteractorPicker::setScalarValue(vtkPolyData *data, double scalarValue){
    int i=0, size = data->GetNumberOfPoints();
    vtkSmartPointer<vtkDoubleArray> scalars = vtkSmartPointer<vtkDoubleArray>::New();
    for(int x=0 ; x<data->GetPointData()->GetNumberOfTuples() ; x++){
        scalars->InsertValue(x, scalarValue);
    }
  data->GetPointData()->SetScalars(scalars);
}

void mafVTKInteractorPicker::setSphereRadius(double sphereRadius){
    m_SphereRadius = sphereRadius;
}

void mafVTKInteractorPicker::setSpherePhiResolution(double spherePhiRes){
    m_SpherePhiRes = spherePhiRes;
}

void mafVTKInteractorPicker::setThetaResolution(double sphereTheRes){
    m_SphereTheRes = sphereTheRes;
}

void mafVTKInteractorPicker::setConeHeight(double coneHeight){
    m_ConeHeight = coneHeight;
}

void mafVTKInteractorPicker::setConeRadius(double coneRadius){
    m_ConeRadius = coneRadius;
}

void mafVTKInteractorPicker::setConeCapping(int coneCapping){
    m_ConeCapping = coneCapping;
}

void mafVTKInteractorPicker::setConeResolution(double coneRes){
    m_ConeRes = coneRes;
}

void mafVTKInteractorPicker::setCylinderHeight(double cylinderHeight){
    m_CylinderHeight = cylinderHeight;
}

void mafVTKInteractorPicker::setCylinderRadius(double cylinderRadius){
    m_CylinderRadius = cylinderRadius;
}

void mafVTKInteractorPicker::setCylinderResolution(double cylinderRes){
    m_CylinderRes = cylinderRes;
}

void mafVTKInteractorPicker::setCubeSide(double XLength, double YLength, double ZLength){
    m_CubeXLength = XLength;
    m_CubeYLength = YLength;
    m_CubeZLength = ZLength;
}

void mafVTKInteractorPicker::setEllipsoidRadius(double ellipsoidYLenght){
    m_EllipsoidYLenght = ellipsoidYLenght;
}

void mafVTKInteractorPicker::setEllipsoidPhiResolution(double ellipsoidPhiRes){
    m_EllipsoidPhiRes = ellipsoidPhiRes;
}

void mafVTKInteractorPicker::setEllipsoidThetaResolution(double ellipsoidTheRes){
    m_EllipsoidTheRes = ellipsoidTheRes;
}
