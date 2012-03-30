/*
 *  mafPluginRegistrator.cpp
 *  mafPluginQt
 *
 *  Created by Paolo Quadrani on 19/03/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPluginRegistrator.h"
#include "mafImporterFieldML.h"

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginFieldML;

mafPluginRegistrator::~mafPluginRegistrator() {
    mafUnregisterObject(mafPluginFieldML::mafImporterFieldML);
}

void mafPluginRegistrator::registerObjects() {
    /// register in maf factory
    mafRegisterObject(mafPluginFieldML::mafImporterFieldML);
    
    mafPluggedObjectsHash pluginHash;

    mafPluggedObjectInformation importerFieldML("Importer for FieldML data", "mafPluginFieldML::mafImporterFieldML");
    pluginHash.insertMulti("mafResources::mafImporter", importerFieldML);


    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafPluggedObjectsHash, pluginHash));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.registerLibrary", mafEventTypeLocal, &argList);
}

mafResources::mafPluginInfo mafPluginRegistrator::pluginInfo() {
    mafResources::mafPluginInfo info;
    info.m_Version = 1.0;
    info.m_PluginName = "FieldML Plugin";
    info.m_Author = "Richard Christie";
    info.m_Vendor = "Auckland Bioengineering Institute";
    info.m_VendorHomepage = "http:///www.fieldml.org/";
    info.m_Description = "FieldML plugin: provide importer and exporter operation";

    return info;
}

Q_EXPORT_PLUGIN2(mafPluginFieldML, mafPluginFieldML::mafPluginRegistrator)
