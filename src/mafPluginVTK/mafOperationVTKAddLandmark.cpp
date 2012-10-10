/*
 *  mafOperationVTKAddLandmark.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 12/07/11.
 *  Copyright 2011 SCS-B3C.s All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafOperationVTKAddLandmark.h"
#include "mafVTKParametricSurfaceSphere.h"
#include "mafInteractorVTKPicker.h"
#include <mafVME.h>
#include <mafDataSet.h>

#include <vtkAlgorithmOutput.h>
#include <vtkSmartPointer.h>
#include <vtkAppendPolyData.h>
#include <vtkCellLocator.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkSphereSource.h>

#define FIXED_MARKER_COLOR  0

using namespace mafPluginVTK;
using namespace mafResources;
using namespace mafEventBus;
using namespace mafCore;

mafOperationVTKAddLandmark::mafOperationVTKAddLandmark(const QString code_location) : mafOperation(code_location), m_Center(NULL), m_LandmarkRadius(0.5), m_ParametricSphere(NULL) {
    //set multi-threaded to off
    m_MultiThreaded = false;

    m_UIFilename = "mafOperationAddLandmark.ui";
    setObjectName("mafOperationAddLandmark");
    m_VMEList.clear();
    m_LandmarkContainerList.clear();
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
    mafDEL(m_ParametricSphere);

    int i = 0;
    for (;i < m_VMEList.count(); i++) {
        mafVME *vme = m_VMEList.at(i);
        mafDEL(vme);
    }
    
    m_VMEList.clear();
}

double mafOperationVTKAddLandmark::radius() const {
    return m_LandmarkRadius;
}

void mafOperationVTKAddLandmark::setRadius(double r) {
    m_LandmarkRadius = r;
}


void mafOperationVTKAddLandmark::vmePicked(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    //Check if ctrl is pressed
    if((modifiers&(1<<MAF_CTRL_KEY))!=0 && m_VMEList.count() != 0){
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
            mafProxy<vtkAlgorithmOutput> *dataSet =  mafProxyPointerTypeCast(vtkAlgorithmOutput, m_VMEList.at(i)->dataSetCollection()->itemAtCurrentTime()->dataValue());
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
           removeLandmark(closestMarkerIndex);
    } else {
        //Create a surface on the picking position
        m_Center = pickPos;
        this->internalUpdate();
    }
}

void mafOperationVTKAddLandmark::execute() {
    fixLandmark();
    m_Status = mafOperationStatusExecuting;
    Q_EMIT executionEnded();
}

void mafOperationVTKAddLandmark::terminated() {
    if (m_Status == mafOperationStatusCanceled || m_Status == mafOperationStatusAborted) {
        unDo();
    }
    mafVME *vme = qobject_cast<mafVME *>(input());
    vme->popInteractor();
}

void mafOperationVTKAddLandmark::unDo() {
    int i = 0;
    for (;i < m_VMEList.count(); i++) {
        mafVME *vme = m_VMEList.at(i);
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafObjectBase *, vme));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.remove", mafEventTypeLocal, &argList);
    }
}

void mafOperationVTKAddLandmark::reDo() {
    int i = 0;
    for (;i < m_VMEList.count(); i++) {
        mafVME *vme = m_VMEList.at(i);
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafObjectBase *, vme));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
    }
}

void mafOperationVTKAddLandmark::setParameters(QVariantList parameters) {
    REQUIRE(parameters.count() == 1);
    m_LandmarkRadius = parameters.at(0).toDouble();
}

void mafOperationVTKAddLandmark::fixLandmark() {
    //Set last marker as fixed.
    mafDEL(m_ParametricSphere);

    if (m_VMEList.count() != 0) {
        mafDataSet *data = m_VMEList.last()->dataSetCollection()->itemAtCurrentTime();
        if(data != NULL){
            //Change color of the fixed marker.
            mafProxy<vtkAlgorithmOutput> *dataSet =  mafProxyPointerTypeCast(vtkAlgorithmOutput, data->dataValue());
            vtkAlgorithm *producer = (*dataSet)->GetProducer();
            vtkDataObject *dataObject = producer->GetOutputDataObject(0);
            vtkPolyData* vtkData = vtkPolyData::SafeDownCast(dataObject);

            this->setScalarValue(vtkData, FIXED_MARKER_COLOR);
            m_VMEList.last()->dataSetCollection()->updateData();
        }
    }
}

void mafOperationVTKAddLandmark::removeLandmark(int index) {
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_VMEList.at(index)));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.remove", mafEventTypeLocal, &argList);

    mafVME *vme = m_VMEList.at(index);
    m_VMEList.removeAt(index);
    mafDEL(vme);
}

void mafOperationVTKAddLandmark::internalUpdate()
{
    if (m_ParametricSphere == NULL){
        //If doesn't exist yet, create a new parametric surface to be used as marker.
        m_ParametricSphere = mafNEW(mafPluginVTK::mafVTKParametricSurfaceSphere);
        m_ParametricSphere->setProperty("sphereRadius", m_LandmarkRadius);

        //vtkAlgorithmOutputs should not be destroyed, so I put them in a list.
        mafCore::mafProxy<vtkAlgorithmOutput> landmarkContainer;
        landmarkContainer = m_ParametricSphere->output();
        const QString codecType("VTK");
        landmarkContainer.setExternalCodecType(codecType);
        landmarkContainer.setClassTypeNameFunction(vtkClassTypeNameExtract);
        m_LandmarkContainerList.append(landmarkContainer);

        //Insert data into VME
        mafVME *landmarkVME = mafNEW(mafResources::mafVME);
        landmarkVME->setObjectName(mafTr("Landmark_%1").arg(m_VMEList.count()));
        mafDataSet * landmarkDataSet = landmarkVME->dataSetCollection()->itemAtCurrentTime();
        
        landmarkDataSet->setDataValue(&m_LandmarkContainerList.last()/*landmarkContainer*/);
        QString dataBoundary = "mafPluginVTK::mafDataBoundaryAlgorithmVTK"; 
        landmarkDataSet->setBoundaryAlgorithmName(dataBoundary);

        landmarkVME->dataSetCollection()->setPose(0., 0., 0.);
        landmarkVME->dataSetCollection()->setOrientation(0., 0., 0.);
        landmarkVME->setProperty("visibility", true);
        m_VMEList.append(landmarkVME);


        // Select input VME before add VME.
        mafCore::mafObjectBase *vme = input();
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafObjectBase*, vme));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.select", mafEventTypeLocal, &argList);

        //Notify vme add
        vme = landmarkVME;
        argList.clear();
        argList.append(mafEventArgument(mafCore::mafObjectBase *, vme));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);

        //Visualize vme added
        argList.clear();
        argList.append(mafEventArgument(mafCore::mafObjectBase*, vme));
        argList.append(mafEventArgument(bool, true));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.sceneNodeShow", mafEventTypeLocal, &argList);
    }
    if(m_ParametricSphere != NULL){
        //Move or set the center of the marker.
        m_ParametricSphere->setCenter(m_Center);
        m_ParametricSphere->updateSurface();
    }
    m_VMEList.last()->dataSetCollection()->updateData();
}

void mafOperationVTKAddLandmark::setScalarValue(vtkPolyData *data, double scalarValue){
    vtkSmartPointer<vtkDoubleArray> scalars = vtkSmartPointer<vtkDoubleArray>::New();
    for(int x=0 ; x<data->GetPointData()->GetNumberOfTuples() ; ++x){
        scalars->InsertValue(x, scalarValue);
    }
    data->GetPointData()->SetScalars(scalars);
    data->Update();
}

void mafOperationVTKAddLandmark::on_nextButton_released() {
    fixLandmark();
}

void mafOperationVTKAddLandmark::on_removeButton_released() {
    if (m_VMEList.count()!= 0) {
        //remove last landmark
        mafDEL(m_ParametricSphere);
        removeLandmark(m_VMEList.count()-1);
    }
}

void mafOperationVTKAddLandmark::on_ALRadius_valueChanged(double radius){
    m_LandmarkRadius = radius;
    if (m_ParametricSphere != NULL) {
        m_ParametricSphere->setProperty("sphereRadius", m_LandmarkRadius);
        m_ParametricSphere->updateSurface();
    }
}