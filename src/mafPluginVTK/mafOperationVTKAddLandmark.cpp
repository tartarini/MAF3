/*
 *  mafOperationVTKAddLandmark.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 12/07/11.
 *  Copyright 2011 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafOperationVTKAddLandmark.h"
#include "mafVTKParametricSurfaceSphere.h"
#include "mafInteractorVTKPicker.h"
#include <mafVME.h>

#include <vtkAlgorithmOutput.h>
#include <vtkSmartPointer.h>
#include <vtkAppendPolyData.h>
#include <vtkCellLocator.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkSphereSource.h>

#define FIXED_MARKER_COLOR  0
#define TMP_MARKER_COLOR    1

using namespace mafPluginVTK;
using namespace mafResources;
using namespace mafEventBus;
using namespace mafCore;

mafOperationVTKAddLandmark::mafOperationVTKAddLandmark(const QString code_location) : mafOperation(code_location), m_Center(NULL), m_AppendData(NULL), m_ParametricSphere(NULL), m_VME(NULL), m_DataSet(NULL) {
    setObjectName("mafOperatiVTKAddLandmark");

    //set multithreaded to off
    m_MultiThreaded = false;

    m_UIFilename = "";
    setObjectName("mafOperationAddLandmark");

    m_PointList.clear();
    m_VMEList.clear();
    this->initializeConnections();
}

bool mafOperationVTKAddLandmark::initialize() {
    bool result = Superclass::initialize();

    mafInteractorVTKPicker *interactor = mafNEW(mafPluginVTK::mafInteractorVTKPicker);
    mafVME *vme = qobject_cast<mafVME *>(input());
    vme->pushInteractor(interactor);
    connect(interactor, SIGNAL(vmePickedVTKSignal(double *, unsigned long , mafCore::mafObjectBase *, QEvent *)), this, SLOT(vmePicked(double *, unsigned long , mafCore::mafObjectBase *, QEvent *)));
    mafDEL(interactor);

    return result;
}

bool mafOperationVTKAddLandmark::acceptObject(mafCore::mafObjectBase *obj) {
    return obj != NULL;
}


mafOperationVTKAddLandmark::~mafOperationVTKAddLandmark(){
    mafUnregisterLocalSignal("maf.local.operation.VTK.nextPick", this, "nextPickSignal()");
    mafUnregisterLocalCallback("maf.local.operation.VTK.nextPick", this, "nextPick()");
    mafDEL(m_ParametricSphere);
    m_AppendData->RemoveAllInputs();
    m_AppendData->Delete();
    m_VMEList.clear();
}

void mafOperationVTKAddLandmark::initializeConnections() {
    // Register API signals.
    mafRegisterLocalSignal("maf.local.operation.VTK.nextPick", this, "nextPickSignal()");

    // Register private callbacks.
    mafRegisterLocalCallback("maf.local.operation.VTK.nextPick", this, "nextPick()");
}

void mafOperationVTKAddLandmark::vmePicked(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    //Check if ctrl is pressed
    if((modifiers&(1<<MAF_CTRL_KEY))!=0){
        //Remove the nearest marker to the picked point.
        double closestPoint[3]; //the coordinates of the closest point will be returned here
        double closestPointDist2; //the squared distance to the closest point will be returned here
        double tmptClosestPointDist2;
        vtkIdType cellId;
        int subId;
        int inputNumber = m_VMEList.count();

        int closestMarkerIndex = 0;
        int i = 0;
        if (inputNumber == 0) {
            return;
        }

        for(; i < inputNumber; ++i){
            vtkSmartPointer<vtkCellLocator> cellLocator = vtkSmartPointer<vtkCellLocator>::New();
            mafProxy<vtkAlgorithmOutput> *dataSet =  mafProxyPointerTypeCast(vtkAlgorithmOutput, m_VMEList.at(i)->dataSetCollection()->itemAt(0)->dataValue());
            vtkAlgorithm *producer = (*dataSet)->GetProducer();
            vtkDataObject *dataObject = producer->GetOutputDataObject(0);
            vtkDataSet* vtkData = vtkDataSet::SafeDownCast(dataObject);

            cellLocator->SetDataSet(vtkData);
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
        if (inputNumber == closestMarkerIndex+1) {
             mafDEL(m_ParametricSphere);
        }
        // Remove closest landmark.
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafObjectBase *, m_VMEList.at(closestMarkerIndex)));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.remove", mafEventTypeLocal, &argList);
        m_PointList.removeAt(closestMarkerIndex);
        mafVME *vme = m_VMEList.at(closestMarkerIndex);
        m_VMEList.removeAt(closestMarkerIndex);
        mafDEL(vme);
    } else {
        //Create a surface on the picking position
        m_Center = pickPos;
        this->internalUpdate();
    }
}

void mafOperationVTKAddLandmark::execute() {
    //Set last pick position as final
    m_PointList.append(m_Center);

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QList<double *>, m_PointList));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.pickedList", mafEventTypeLocal, &argList);

    m_Status = mafOperationStatusExecuting;
    mafDEL(m_DataSet);
    mafDEL(m_VME);
    emit executionEnded();
}

void mafOperationVTKAddLandmark::terminated() {
    if (m_Status == mafOperationStatusCanceled || m_Status == mafOperationStatusAborted) {
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafObjectBase *, m_VME));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.remove", mafEventTypeLocal, &argList);
    }

    mafVME *vme = qobject_cast<mafVME *>(input());
    vme->popInteractor();

    //Superclass::terminated();
}

void mafOperationVTKAddLandmark::unDo() {
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_Output));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.remove", mafEventTypeLocal, &argList);
}

void mafOperationVTKAddLandmark::reDo() {
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_Output));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
}

void mafOperationVTKAddLandmark::setParameters(QVariantList parameters) {
    //@@TODO NEED TO IMPLEMENT
}

void mafOperationVTKAddLandmark::nextPick() {
    //Set last marker as fixed.
    mafDEL(m_ParametricSphere);
    m_PointList.append(m_Center);

    /*vtkPolyData *data = NULL;
    int n = m_AppendData->GetNumberOfInputConnections(0);
    data = m_AppendData->GetInput(n-1);*/
    if(m_DataSet != NULL){
        //Change color of the fixed marker.
        mafProxy<vtkAlgorithmOutput> *dataSet =  mafProxyPointerTypeCast(vtkAlgorithmOutput, m_DataSet->dataValue());
        vtkAlgorithm *producer = (*dataSet)->GetProducer();
        vtkDataObject *dataObject = producer->GetOutputDataObject(0);
        vtkPolyData* vtkData = vtkPolyData::SafeDownCast(dataObject);

        this->setScalarValue(vtkData, FIXED_MARKER_COLOR);
    }
    //m_AppendData->Modified(); //to update scalar of input polydata
}


void mafOperationVTKAddLandmark::internalUpdate()
{
    if (m_ParametricSphere == NULL){
        //If doesn't exist yet, create a new parametric surface to be used as marker.
        m_ParametricSphere = mafNEW(mafPluginVTK::mafVTKParametricSurfaceSphere);
        m_ParametricSphere->setProperty("sphereRadius", 0.5);

        m_LandmarkContainer.setExternalCodecType("VTK");
        m_LandmarkContainer.setClassTypeNameFunction(vtkClassTypeNameExtract);
        m_LandmarkContainer = m_ParametricSphere->output();

        //Insert data into VME
        m_VME = mafNEW(mafResources::mafVME);
        m_VME->setObjectName(mafTr("Landmark_%1").arg(m_VMEList.count()));
        m_DataSet = mafNEW(mafResources::mafDataSet);
        m_DataSet->setBoundaryAlgorithmName("mafPluginVTK::mafDataBoundaryAlgorithmVTK");
        m_DataSet->setDataValue(&m_LandmarkContainer);
        m_VME->dataSetCollection()->insertItem(m_DataSet, 0);
        m_VME->dataSetCollection()->setPose(0., 0., 0.);
        m_VME->dataSetCollection()->setOrientation(0., 0., 0.);
        m_VME->setProperty("visibility", true);
        m_VMEList.append(m_VME);
        //this->m_Output = m_VME;

        //Notify vme add
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafObjectBase *, m_VME));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
    }
    if(m_ParametricSphere != NULL){
        //Move or set the center of the marker.
        m_ParametricSphere->setCenter(m_Center);
        m_ParametricSphere->updateSurface();

        /*vtkPolyData *data = NULL;
        int n = m_AppendData->GetNumberOfInputConnections(0);
        data = m_AppendData->GetInput(n-1);
        if(data != NULL){
            //Change color of the fixed marker.
            this->setScalarValue(data, TMP_MARKER_COLOR);
        }*/
    }
    m_VME->dataSetCollection()->updateData();
}

void mafOperationVTKAddLandmark::setScalarValue(vtkPolyData *data, double scalarValue){
    vtkSmartPointer<vtkDoubleArray> scalars = vtkSmartPointer<vtkDoubleArray>::New();
    for(int x=0 ; x<data->GetPointData()->GetNumberOfTuples() ; ++x){
        scalars->InsertValue(x, scalarValue);
    }
    data->GetPointData()->SetScalars(scalars);
}
