/*
 *  mafImporterVTK.cpp
 *  mafPluginVTK
 *
 *  Created by Daniele Giunchi on 04/04/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafImporterOutOfCoreVolume.h"
#include <mafVME.h>
#include <mafDataSet.h>

using namespace mafEventBus;
using namespace mafPluginOutOfCore;
using namespace mafResources;

mafImporterOutOfCoreVolume::mafImporterOutOfCoreVolume(const QString code_location) : mafImporter(code_location) {
    m_MultiThreaded = false;
}

mafImporterOutOfCoreVolume::~mafImporterOutOfCoreVolume() {
}

void mafImporterOutOfCoreVolume::execute() {
    QMutex mutex(QMutex::Recursive);
    QMutexLocker locker(&mutex);

    m_Status = mafOperationStatusExecuting;
    
    checkImportFile();
    if (m_Status == mafOperationStatusAborted) {
        cleanup();
        return;
    }

    QString encodeType = "RAW";
    mafCore::mafMemento *memento = 0;

    QString volumeFileName = filename();

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, volumeFileName));
    argList.append(mafEventArgument(QString, encodeType));
    QGenericReturnArgument retVal = mafEventReturnArgument(mafCore::mafMemento*, memento);
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.load", mafEventTypeLocal, &argList, &retVal);

    /*
    QString encodeType = "VOLUME_LOD";
    mafCore::mafMemento *memento = 0;

    QString volumeFileName = filename();

    mafVolume *volume = mafNEW(mafPluginOutOfCore::mafVolume);
    mafCore::mafProxy<mafVolume> container;
    container = (mafVolume*)volume;

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafProxyInterface *, &container));
    argList.append(mafEventArgument(QString, volumeFileName));
    argList.append(mafEventArgument(QString, encodeType));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.update", mafEventTypeLocal, &argList);

    */

    // verify memento
    mafVolume *volume = mafNEW(mafPluginOutOfCore::mafVolume);
    volume->setMemento(memento);
    QString fn = volumeFileName.mid(volumeFileName.lastIndexOf("/")+1,volumeFileName.lastIndexOf(".") - volumeFileName.lastIndexOf("/")).append(".raw");
    //volume->setFileName(fn);
    mafDEL(memento);

    char *v = fn.toAscii().data();

    m_ImportedData = volume;
    m_ImportedData.setExternalCodecType("VOLUME_LOD");
    //m_ImportedData.setClassTypeNameFunction(vtkClassTypeNameExtract);
    m_ImportedData.setExternalDataType(QString("mafVolume"));

    //here set the mafDataset with the VTK data
    importedData(&m_ImportedData);
    
    //set the default boundary algorithm for VTK vme
    mafResources::mafVME * vme = qobject_cast<mafResources::mafVME *> (this->m_Output);
    //vme->dataSetCollection()->itemAtCurrentTime()->setBoundaryAlgorithmName("mafPluginVTK::mafDataBoundaryAlgorithmVTK");

    vme->setProperty("iconType", "mafVMEVolumeVTK"); // need to change icon even if probably it will be the same pic

    volume->setMemoryLimit(64 * 1024);
    volume->updateDataValue();

    Q_EMIT executionEnded();
}
