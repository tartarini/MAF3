/*
 *  mafPluginRegistrator.cpp
 *  mafPluginQt
 *
 *  Created by Matteo Giacomoni on 03/07/12.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPluginRegistrator.h"


using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginZip;


mafPluginRegistrator::~mafPluginRegistrator() 
{
  
}

void mafPluginRegistrator::registerObjects() 
{
  /// register in maf factory
    
    
  mafPluggedObjectsHash pluginHash;

  mafEventBus::mafEventArgumentsList argList;
  
  argList.append(mafEventArgument(mafCore::mafPluggedObjectsHash, pluginHash));
  mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.registerLibrary", mafEventTypeLocal, &argList);
}

mafResources::mafPluginInfo mafPluginRegistrator::pluginInfo() {
    mafResources::mafPluginInfo info;
    info.m_Version = 1.0;
    info.m_PluginName = "Plugin Qt";
    info.m_Author = "Matteo Giacomoni";
    info.m_Vendor = "SCS-B3C";
    info.m_VendorHomepage = "http:///www.openmaf.org/";
    info.m_Description = "Qt plugin: created only for be compliant to Qt plugins";

    return info;
}

Q_EXPORT_PLUGIN2(mafPluginZip, mafPluginZip::mafPluginRegistrator)
