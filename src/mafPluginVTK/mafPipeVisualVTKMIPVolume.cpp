/*
 *  mafPipeVisualVTKMIPVolume.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 10/05/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisualVTKMIPVolume.h"
#include "mafVTKWidget.h"
#include <mafVME.h>
#include <mafDataSet.h>
#include <mafDataSetCollection.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkVolumeRayCastMIPFunction.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkAlgorithm.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkVolume.h>
#include <vtkAlgorithmOutput.h>
#include <vtkInformation.h>

#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkProperty.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;
using namespace std;

mafPipeVisualVTKMIPVolume::mafPipeVisualVTKMIPVolume(const QString code_location) : mafPipeVisualVTK(code_location), m_Mapper(NULL) {
    m_UIFilename = "mafPipeVisualMIPVolume.ui";
    
    // Create transfer mapping scalar value to opacity
    m_PieceWiseTransferFunction = vtkPiecewiseFunction::New();
    //vtkColorTransferFunction* colorTransferFunction = vtkColorTransferFunction::New();
     
    // The property describes how the data will look
    vtkVolumeProperty* volumeProperty = vtkVolumeProperty::New();
    //volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(m_PieceWiseTransferFunction);
    volumeProperty->ShadeOn();
    volumeProperty->SetInterpolationTypeToNearest();
    volumeProperty->SetInterpolationTypeToLinear();
    
    m_Mapper = vtkVolumeRayCastMapper::New();
    vtkVolumeRayCastMIPFunction* rcFunction = vtkVolumeRayCastMIPFunction::New();
    m_Prop3D = vtkVolume::New();
    m_Prop3D.setDestructionFunction(&vtkActor::Delete);
    m_Mapper->SetVolumeRayCastFunction(rcFunction);
    vtkVolume::SafeDownCast(m_Prop3D)->SetMapper(m_Mapper);
    rcFunction->Delete(); 
    rcFunction = NULL;
    vtkVolume::SafeDownCast(m_Prop3D)->SetProperty(volumeProperty);
    volumeProperty->Delete(); 
    volumeProperty = NULL;
    m_Output = &m_Prop3D;
}

mafPipeVisualVTKMIPVolume::~mafPipeVisualVTKMIPVolume() {
    m_Mapper->Delete();
}

bool mafPipeVisualVTKMIPVolume::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        QString dataType = vme->dataSetCollection()->itemAtCurrentTime()->externalDataType();
        if(dataType.startsWith("vtkStructuredPoints", Qt::CaseSensitive)) {
            return true;
        }
    }
    return false;
}

void mafPipeVisualVTKMIPVolume::updatePipe(double t) {
    Superclass::updatePipe(t);

    mafDataSet *data = dataSetForInput(0, t);
    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, data->dataValue());

    vtkAlgorithm *producer = (*dataSet)->GetProducer();
    vtkDataObject *dataObject = producer->GetOutputDataObject(0);
    vtkDataSet* vtkData = vtkDataSet::SafeDownCast(dataObject);

    //Get data contained in the mafProxy
    m_Mapper->SetInput(vtkData);
    updateFunctions(vtkData);
    
    m_Mapper->Update();
}

void mafPipeVisualVTKMIPVolume::updateFunctions(vtkDataSet *dataset) {
    m_PieceWiseTransferFunction->RemoveAllPoints();
    
    int tv = 128;
    double sr[2],w,p;
    dataset->GetScalarRange(sr);
    w = sr[1] - sr[0];
    for (int v=0;v<tv;v++) {
        p = v*w/tv+sr[0];
        m_PieceWiseTransferFunction->AddPoint(p, (double)v/(double)tv);
    }
    
    m_PieceWiseTransferFunction->Update();
}
