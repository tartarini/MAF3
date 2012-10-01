/*
 *  mafPluginRegistrator.h
 *  mafPluginTest
 *
 *  Created by Roberto Mucci on 14/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINTESTREGISTRATOR_H
#define MAFPLUGINTESTREGISTRATOR_H

#include "mafPluginTestDefinitions.h"

namespace mafPluginTest {

/**
 Class name: mafPluginRegistrator
 This class is the object that is in charge to register all the class contained into the library as plug-in
 by sending the message through the mafEventBus using the macro mafRegisterPluginEvent.
 @sa mafEventBusManager, mafPluginManager
 */
class MAFPLUGINTESTSHARED_EXPORT mafPluginRegistrator {
public:
    /// Object constructor
    mafPluginRegistrator();

    /// Object destructor
    ~mafPluginRegistrator();

    /// Register all the objects present into the library.
    void registerObjects();
};

/** Retrieve the engine version we're going to expect*/
extern "C" MAFPLUGINTESTSHARED_EXPORT mafResources::mafPluginInfo pluginInfo() {
    mafResources::mafPluginInfo info;
    info.m_Version = 1.0;
    info.m_PluginName = "Plugin Test";
    info.m_Author = "Paolo Quadrani";
    info.m_Vendor = "SCS-B3C";
    info.m_VendorHomepage = "http:///www.biomedtown.org/";
    info.m_Description = "Test plugin: created only for testing plugin manager";

    return info;
}

// MAF required version function.
// XML file for the UI. Also with switch possibility between different levels of complexity.
mafPluginRegistrator registrator;

/** Tells us to register our functionality to an engine kernel*/
extern "C" MAFPLUGINTESTSHARED_EXPORT void registerPlugin() {
    registrator.registerObjects();
}

} // namespace mafPluginTest

#endif // MAFPLUGINTESTREGISTRATOR_H
