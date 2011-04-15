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

mafImporter::mafImporter(const QString code_location) : mafOperation(code_location), m_Filename("") {
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
}

void mafImporter::checkImportFile() {
    if (m_Filename.isEmpty()) {
        qWarning() << mafTr("Filename of data to import is needed.");
        m_Abort = true;
    } else if (!QFile::exists(m_Filename)) {
        qCritical() << m_Filename << mafTr(" doesn't exists!!.");
        m_Abort = true;
    }
}

void mafImporter::importedData(mafCore::mafContainerInterface *data) {
    QFileInfo info(m_Filename);
    
    //Insert data into VME
    this->m_Output = mafNEW(mafResources::mafVME);
    this->m_Output->setObjectName(info.baseName());
    mafDataSet *dataSet = mafNEW(mafResources::mafDataSet);
    dataSet->setDataValue(data);
    ((mafVME *)this->m_Output)->dataSetCollection()->insertItem(dataSet, 0);
    mafDEL(dataSet);
    
    //Notify vme add
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_Output));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);
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
