/*
 *  mafPluginRegistrator.h
 *  mafPluginCTK
 *
 *  Created by Daniele Giunchi && Roberto Mucci on 14/12/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINREGISTRATOR_H
#define MAFPLUGINREGISTRATOR_H

#include "mafPluginOutOfCoreDefinitions.h"


namespace mafPluginOutOfCore {

/**
 Class name: mafPluginRegistrator
 This class is the object that is in charge to register all the class contained into the library as plug-in
 by sending the message through the mafEventBus using the macro mafRegisterPluginEvent.
 @sa mafEventBusManager, mafPluginManager
 */
class MAFPLUGINOUTOFCORESHARED_EXPORT mafPluginRegistrator {
public:
    /// Object constructor
    mafPluginRegistrator();

    /// Object destructor
    ~mafPluginRegistrator();

    /// Register all the objects present into the library.
    void registerObjects();

private:
    /// Register the all the objects contained into the plug-in
    void registerAllObjects();
};

/** Retrieve the engine version we're going to expect*/
extern "C" MAFPLUGINOUTOFCORESHARED_EXPORT mafResources::mafPluginInfo pluginInfo() {
    mafResources::mafPluginInfo info;
    info.m_Version = 1.0;
    info.m_PluginName = "Plugin OutOfCore";
    info.m_Author = "Daniele Giunchi";
    info.m_Vendor = "SCS";
    info.m_VendorHomepage = "http:///www.biomedtown.org/";
    info.m_Description = "Plugin library which contain elements for out of core";

    return info;
}

// MAF required version function.
// XML file for the UI. Also with switch possibility between different levels of complexity.
mafPluginRegistrator registrator;

/** Tells us to register our functionality to an engine kernel*/
extern "C" MAFPLUGINOUTOFCORESHARED_EXPORT void registerPlugin() {
    registrator.registerObjects();
}

} // namespace mafPluginOutOfCore

#endif // MAFPLUGINREGISTRATOR_H
