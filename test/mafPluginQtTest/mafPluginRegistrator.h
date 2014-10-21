/*
 *  mafPluginRegistrator.h
 *  mafPluginQt
 *
 *  Created by Paolo Quadrani on 19/03/12.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINQTREGISTRATOR_H
#define MAFPLUGINQTREGISTRATOR_H

#include "mafPluginQtTestDefinitions.h"

namespace mafPluginQtTest {

/**
 Class name: mafPluginRegistrator
 This class is the object that is in charge to register all the class contained into the library as plug-in
 by sending the message through the mafEventBus using the macro mafRegisterPluginEvent.
 @sa mafEventBusManager, mafPluginManager
 */
class MAFPLUGINQTTESTSHARED_EXPORT mafPluginRegistrator : public QObject, mafResources::mafPluginQtInterface {
    Q_OBJECT
    Q_INTERFACES(mafResources::mafPluginQtInterface)
	Q_PLUGIN_METADATA(IID "mafPluginQtTest" FILE "mafPluginQtTest.json")
    
public:
    /// Object destructor
    /*virtual*/ ~mafPluginRegistrator();

    /*virtual*/ mafResources::mafPluginInfo pluginInfo();
    
    /// Register all the objects present into the library.
    /*virtual*/ void registerObjects();
};

/** Retrieve the engine version we're going to expect*/
//extern "C" MAFPLUGINQTSHARED_EXPORT mafResources::mafPluginInfo pluginInfo() {
//    mafResources::mafPluginInfo info;
//    info.m_Version = 1.0;
//    info.m_PluginName = "Plugin Qt";
//    info.m_Author = "Paolo Quadrani";
//    info.m_Vendor = "SCS-B3C";
//    info.m_VendorHomepage = "http:///www.biomedtown.org/";
//    info.m_Description = "Qt plugin: created only for testing plugin manager";
//
//    return info;
//}

// MAF required version function.
// XML file for the UI. Also with switch possibility between different levels of complexity.
//mafPluginRegistrator registrator;

/** Tells us to register our functionality to an engine kernel*/
//extern "C" MAFPLUGINQTSHARED_EXPORT void registerPlugin() {
//    registrator.registerObjects();
//}

} // namespace mafPluginTest

#endif // MAFPLUGINQTREGISTRATOR_H
