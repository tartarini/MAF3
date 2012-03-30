/*
 *  mafImporterFieldML.cpp
 *  mafPluginFieldML
 *
 *  Created by Paolo Quadrani on 16/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafImporterFieldML.h"
#include <mafVME.h>
#include <mafDataSet.h>

//fieldML inclusion files
#include <fieldml_api.h>
#include <FieldmlIoApi.h>

using namespace mafPluginFieldML;
using namespace mafResources;

mafImporterFieldML::mafImporterFieldML(const QString code_location) : mafImporter(code_location) {
    //m_Reader = vtkSTLReader::New();
    m_MultiThreaded = false;
    setProperty("wildcard", mafTr("FML Files (*.*)"));
}

mafImporterFieldML::~mafImporterFieldML() {
    //m_Reader->Delete();
}

void mafImporterFieldML::execute() {
    QMutex mutex(QMutex::Recursive);
    QMutexLocker locker(&mutex);

    m_Status = mafOperationStatusExecuting;
    
    checkImportFile();
    if (m_Status == mafOperationStatusAborted) {
        cleanup();
        return;
    }
    
    QByteArray ba = filename().toAscii();
    
    /// experimental code
    FmlSessionHandle fmlSession = Fieldml_CreateFromFile(ba.constData());
    
    if (fmlSession == (const FmlSessionHandle)FML_INVALID_HANDLE)
	{
	    qDebug() << "Read FieldML: could not parse file " << ba.constData();
	}
	else
	{
	    qDebug() << "Session Valid!!!!";
		int parseErrorCount = Fieldml_GetErrorCount(fmlSession);
		int return_code = (parseErrorCount == 0);
		for (int i = 1; i <= parseErrorCount; i++)
		{
			char *error_string = Fieldml_GetError(fmlSession, i);
     	    qDebug() << "FieldML Parse error: " << error_string;
			Fieldml_FreeString(error_string);
			
		}
		// fill m_ImportedData with some information for now.
		const int meshCount = Fieldml_GetObjectCount(fmlSession, FHT_MESH_TYPE);
		m_DataInformation.numberOfMesh = meshCount;
		
		
		FmlObjectHandle fmlMeshType = Fieldml_GetObject(fmlSession, FHT_MESH_TYPE, 1);
		
		char *meshName = Fieldml_GetObjectName(fmlSession, fmlMeshType);
		qDebug() << "Mesh of this type: " << meshName;
        Fieldml_FreeString(meshName);
        
		FmlObjectHandle fmlMeshChartType = Fieldml_GetMeshChartType(fmlSession, fmlMeshType);
		FmlObjectHandle fmlMeshChartComponentType = Fieldml_GetTypeComponentEnsemble(fmlSession, fmlMeshChartType);
		int meshDimension = Fieldml_GetMemberCount(fmlSession, fmlMeshChartComponentType);
		m_DataInformation.firstMeshDimension = meshDimension;
		
		m_ImportedData = &m_DataInformation;
		
		importedData(&m_ImportedData);
		

		
		Fieldml_Destroy(fmlSession);
	}
       
    /// end experimental code
    
    
    //m_Reader->SetFileName(ba.constData());
    //m_Reader->Update();
    
    //m_ImportedData = m_Reader->GetOutputPort();
    //m_ImportedData.setExternalCodecType("VTK");
    //m_ImportedData.setClassTypeNameFunction(vtkClassTypeNameExtract);
    //QString dataType = m_ImportedData.externalDataType();
    
    //here set the mafDataset with the VTK data
    //importedData(&m_ImportedData);
    
    //set the default boundary algorithm for VTK vme
    //mafResources::mafVME * vme = qobject_cast<mafResources::mafVME *> (this->m_Output);
    //vme->dataSetCollection()->itemAtCurrentTime()->setBoundaryAlgorithmName("mafPluginVTK::mafDataBoundaryAlgorithmVTK");

    //if (dataType.compare("vtkPolyData") == 0) {
    //    vme->setProperty("iconType", "mafVMESurfaceVTK");
    //} else {
    //    vme->setProperty("iconType", "mafVMEVolumeVTK");
    //}
    
    Q_EMIT executionEnded();
}
