/*
 *  mafImporter.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 04/04/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafImporter.h"
#include "mafVME.h"
#include "mafDataSet.h"

#include <mafContainer.h>

using namespace mafResources;
using namespace mafEventBus;
using namespace mafCore;

mafImporter::mafImporter(const QString code_location) : mafOperation(code_location), m_EncodeType(""), m_Filename(""), m_DataImported(false) {
    m_UIFilename = "mafImporter.ui";
}

mafImporter::~mafImporter() {
    cleanup();
}

bool mafImporter::acceptObject(mafCore::mafObjectBase *obj) {
    return true;
}

void mafImporter::setParameters(QVariantList parameters) {
    m_Filename = parameters.at(0).toString();
}

void mafImporter::cleanup() {
    // Cleanup memory and deregister callback.
    mafDEL(m_Output);
    mafUnregisterLocalCallback("maf.local.serialization.extDataImported", this, "importedData(mafCore::mafContainerInterface *)")
}

void mafImporter::requestToImportFile() {
    if (m_Filename.isEmpty()) {
        qWarning() << mafTr("Filename of data to import is needed.");
    } else {
        mafRegisterLocalCallback("maf.local.serialization.extDataImported", this , "importedData(mafCore::mafContainerInterface *)")
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(QString, m_Filename));
        argList.append(mafEventArgument(QString, m_EncodeType));
        mafEventBusManager::instance()->notifyEvent("maf.local.serialization.import", mafEventTypeLocal, &argList);
    }
}

void mafImporter::importedData(mafCore::mafContainerInterface *data) {
    mafUnregisterLocalCallback("maf.local.serialization.extDataImported", this, "importedData(mafCore::mafContainerInterface *)")

    QFileInfo info(m_Filename);
    
    //Insert data into VME
    m_VME = mafNEW(mafResources::mafVME);
    m_VME->setObjectName(info.baseName());
    mafDataSet *dataSet = mafNEW(mafResources::mafDataSet);
    dataSet->setDataValue(data);
    m_VME->dataSetCollection()->insertItem(dataSet, 0);
    mafDEL(dataSet);
    this->m_Output = m_VME;

    //Notify vme add
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_Output));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
    
    m_DataImported = true;
}

void mafImporter::unDo() {
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_Output));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.remove", mafEventTypeLocal, &argList);
}

void mafImporter::reDo() {
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_Output));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
}
