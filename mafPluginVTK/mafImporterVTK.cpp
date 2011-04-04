/*
 *  mafImporterVTK.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 04/04/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafImporterVTK.h"
#include <mafContainer.h>
#include <vtkAlgorithmOutput.h>

using namespace mafPluginVTK;
using namespace mafResources;
using namespace mafEventBus;
using namespace mafCore;

mafImporterVTK::mafImporterVTK(const QString code_location) : mafOperation(code_location), m_Filename(""), m_DataSet(NULL), m_VME(NULL), m_DataImported(false) {
    m_OperationType = mafOperationTypeImporter;
    m_UIFilename = "mafImporterVTK.ui";
    mafRegisterLocalCallback("maf.local.serialization.extDataImported", this , "importedVTKData(mafCore::mafContainerInterface *)")
}

mafImporterVTK::~mafImporterVTK() {
    cleanup();
}

bool mafImporterVTK::acceptObject(mafCore::mafObjectBase *obj) {
    return true;
}

void mafImporterVTK::cleanup() {
    // Cleanup memory and deregister callback.
    mafDEL(m_DataSet);
    mafDEL(m_Output);
    mafUnregisterLocalCallback("maf.local.serialization.extDataImported", this, "importedVTKData(mafCore::mafContainerInterface *)")
}

void mafImporterVTK::execute() {
    if (m_Filename.isEmpty()) {
        qWarning() << mafTr("Filename of data to import is needed.");
    } else {
        QString encodeType = "VTK";
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(QString, m_Filename));
        argList.append(mafEventArgument(QString, encodeType));
        mafEventBusManager::instance()->notifyEvent("maf.local.serialization.import", mafEventTypeLocal, &argList);
        while (!m_DataImported || !m_Abort) {
            ;
        }
        if (m_Abort) {
            cleanup();
            return;
        }
    }
    
    emit executionEnded();
}

void mafImporterVTK::importedVTKData(mafCore::mafContainerInterface *data) {
    mafUnregisterLocalCallback("maf.local.serialization.extDataImported", this, "importedVTKData(mafCore::mafContainerInterface *)")

    //Insert data into VME
    m_VME = mafNEW(mafResources::mafVME);
    m_VME->setObjectName(mafTr("Parametric Surface"));
    m_DataSet = mafNEW(mafResources::mafDataSet);
    m_DataSet->setDataValue(data);
    m_VME->dataSetCollection()->insertItem(m_DataSet, 0);
    this->m_Output = m_VME;

    //Notify vme add
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_Output));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
    
    m_DataImported = true;
}

void mafImporterVTK::unDo() {
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_Output));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.remove", mafEventTypeLocal, &argList);
}

void mafImporterVTK::reDo() {
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_Output));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
}
