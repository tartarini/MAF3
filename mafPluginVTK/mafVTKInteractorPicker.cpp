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
#include "mafVTKParametricSurfaceSphere.h"

#include <vtkAlgorithmOutput.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkAppendPolyData.h>
#include <vtkCellLocator.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>


#define FIXED_MARKER_COLOR  0
#define TMP_MARKER_COLOR    1

using namespace mafCore;
using namespace mafEventBus;
using namespace mafPluginVTK;

mafVTKInteractorPicker::mafVTKInteractorPicker(const mafString code_location) : mafResources::mafInteractor(code_location), m_Mapper(NULL), m_AppendData(NULL), m_ParametricSurface(NULL), m_Output(NULL) {
    m_Center = NULL;
    m_PointList.clear();
    this->initializeConnections();
}

mafVTKInteractorPicker::~mafVTKInteractorPicker(){
    mafDEL(m_ParametricSurface);
}

void mafVTKInteractorPicker::initializeConnections() {
    this->createPipe();

    mafIdProvider::instance()->createNewId("maf.local.resources.interaction.vmePick");

    // Register API signals.
    mafRegisterLocalSignal("maf.local.resources.interaction.vmePick", this, "vmePickSignal(double *, unsigned long, mafCore::mafContainerInterface *, QEvent *)");

    // Register private callbacks.
    mafRegisterLocalCallback("maf.local.resources.interaction.vmePick", this, "vmePick(double *, unsigned long, mafCore::mafContainerInterface *, QEvent *)");

    // Register API signals.
    mafRegisterLocalSignal("maf.local.operation.VTK.nextPick", this, "nextPickSignal()");

    // Register private callbacks.
    mafRegisterLocalCallback("maf.local.operation.VTK.nextPick", this, "nextPick()");

    // Register API signals.
    mafRegisterLocalSignal("maf.local.operation.VTK.OK", this, "OKSignal()");

    // Register private callbacks.
    mafRegisterLocalCallback("maf.local.operation.VTK.OK", this, "OK()");
}

void mafVTKInteractorPicker::createPipe() {
    m_Mapper = vtkPolyDataMapper::New();

    //vtkAppendPolyData is used to create a unique polydata for all markers.
    m_AppendData = vtkAppendPolyData::New();
    m_Mapper->SetScalarRange(FIXED_MARKER_COLOR-1, TMP_MARKER_COLOR);
    m_Mapper->SetInputConnection(m_AppendData->GetOutputPort());

    m_Actor = vtkActor::New();
    m_Actor.setDestructionFunction(&vtkActor::Delete);
    m_Actor->SetMapper(m_Mapper);
    m_Output = &m_Actor;
}

void mafVTKInteractorPicker::vmePick(double *pickPos, unsigned long modifiers, mafCore::mafContainerInterface *actor, QEvent *e ) {
    //Check if ctrl is pressed
    if((modifiers&(1<<MAF_CTRL_KEY))!=0){
        //Remove the nearest marker to the picked point.
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
        // If the last marker is removed, then delete the current parametric surface.
        if(m_ParametricSurface->output() == m_AppendData->GetInputConnection(0,closestMarkerIndex)){
            mafDEL(m_ParametricSurface);
        }
        // Remove marker frome the AppendPolyData.
        m_AppendData->RemoveInputConnection(0,m_AppendData->GetInputConnection(0,closestMarkerIndex));
        m_PointList.removeAt(closestMarkerIndex);
    } else {
        //Create a surface on the picking position
        m_Center = pickPos;
        this->internalUpdate();
    }
}

void mafVTKInteractorPicker::nextPick() {
    //Set last marker as fixed.
    mafDEL(m_ParametricSurface);
    m_ParametricSurface = NULL;
    m_PointList.append(m_Center);
    vtkPolyData *data = NULL;
    int n = m_AppendData->GetNumberOfInputConnections(0);
    data = m_AppendData->GetInput(n-1);
    if(data != NULL){
        //Change color of the fixed marker.
        this->setScalarValue(data, FIXED_MARKER_COLOR);
    }
    m_AppendData->Modified(); //to update scalar of input polydata
}

void mafVTKInteractorPicker::OK() {
    //Set last pick position as final
    m_PointList.append(m_Center);

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafList<double *>, m_PointList));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.pickedList", mafEventTypeLocal, &argList);
}

void mafVTKInteractorPicker::internalUpdate()
{
    if (m_ParametricSurface == NULL){
        //If doesn't exist yet, create a new parametric surface to be used as marker.
        m_ParametricSurface = (mafVTKParametricSurface *)mafNEWFromString(m_ParametricSurfaceType);
        m_AppendData->AddInputConnection(m_ParametricSurface->output());
        m_AppendData->Modified(); //to update scalar of input polydata
    }
    if(m_ParametricSurface != NULL){
        //Move or set the center of the marker.
        m_ParametricSurface->setCenter(m_Center);
        m_ParametricSurface->updateSurface();

        vtkPolyData *data = NULL;
        int n = m_AppendData->GetNumberOfInputConnections(0);
        data = m_AppendData->GetInput(n-1);
        if(data != NULL){
            //Change color of the fixed marker.
            this->setScalarValue(data, TMP_MARKER_COLOR);
        }
    }
}

void mafVTKInteractorPicker::setScalarValue(vtkPolyData *data, double scalarValue){
    vtkSmartPointer<vtkDoubleArray> scalars = vtkSmartPointer<vtkDoubleArray>::New();
    for(int x=0 ; x<data->GetPointData()->GetNumberOfTuples() ; x++){
        scalars->InsertValue(x, scalarValue);
    }
  data->GetPointData()->SetScalars(scalars);
}

