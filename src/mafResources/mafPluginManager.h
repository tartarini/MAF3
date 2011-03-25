/*
 *  mafPluginManager.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINMANAGER_H
#define MAFPLUGINMANAGER_H

// Includes list
#include "mafResourcesDefinitions.h"

namespace mafResources {

// Class forwarding list
class mafPlugin;

/**
Class name: mafPluginManager
This class provides the engine for loading plug-ins and define the following topics:
- maf.local.resources.plugin.loadLibrary which allows you to load a shared library.
- maf.local.resources.plugin.registerLibrary which is used by external libraries to register their plugged objects.
- maf.local.resources.plugin.resourcesQuery which allows you to query for plugged objects that extend the given base class.
*/
/** mafPluginManager - The engine's system core.
To load a plug-in simply call the loadPlugin method by passing as argument the
library name related to the plugin to load. Another way to load the plugin is to
ask the mafEventBusManager to notify the maf.local.resources.plugin.loadLibrary
event with one argument containing the library name to load. This will trigger the
loading of the plugin library from the mafPluginManager.
Each plugin library emits the event maf.local.resources.plugin.registerLibrary,
which ask the plugin manager to register it.
After that a plugin has been loaded and then registered, the plugin list can be
queried through the event associated to the maf.local.resources.plugin.resourcesQuery
topic. For and example of mafPluginManager usage take a look at the mafPluginManagerTest
located into the mafResourcesTest directory.
*/
class MAFRESOURCESSHARED_EXPORT mafPluginManager : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Return an instance of the Manager
    static mafPluginManager *instance();

    /// Destroy the singleton instance. To be called at the end of the application.
    void shutdown();

    /// Return the Plug-in information structure
    mafPluginInfo pluginInformation(QString plugin_name);

signals:
    /// Signal emitted by the external library that wants to register its own objects.
    void registerPluginToManager(mafCore::mafPluggedObjectsHash pluginHash);

    /// Signal emitted by logic to load an external library containing plug-ins.
    void loadPluginLibrary(const QString &pluginFilename);

    /// Signal emitted to query for plugins that extend a given base class type.
    mafCore::mafPluggedObjectInformationList *queryPluggedObjectsSignal(const QString &baseMAFClassExtended);

public slots:
    /// Loads a plugin
    void loadPlugin(const QString &pluginFilename);

    /// Unload the plug-in and remove it from the hash.
//    void unLoadPlugin(const QString &pluginFilename);

    /// Plug-in registration callback.
    /** This method is called by the REGISTER_PLUGIN signal and receive as parameter the hash containing the information on the class type of the base MAF class
    (mafResources::mafView, mafResources::mafOperation, mafResources::mafVisualPipe...), the type of the plugged object that extend the base class
    and the default label that you want to be shown at the user interface level.*/
    void registerPlugin(mafCore::mafPluggedObjectsHash pluginHash);

    /// Allow to query the list of plugged object according to the base MAF class type given as argument. The list given as parameter will be filled with the query result.
    mafCore::mafPluggedObjectInformationList *queryPluggedObjects(const QString &baseMAFClassExtended);

    /// Allow to query the base MAF class type that the plugged object is extending.
    void queryBaseClassType(QString &pluggedObjectClassType, mafCore::mafPluggedObjectInformationList *resultBaseClass);

protected:
    /// Object destructor
    /*virtual*/ ~mafPluginManager();

private:
    /// Object costructor
    mafPluginManager(const QString code_location = "");

    /// Hash of plugins by their associated file names
    typedef QHash<QString, mafPlugin *> mafPluginHash;

    mafPluginHash m_PluginsHash;  ///< Association of filenames with all plugins handlers.
    mafCore::mafPluggedObjectsHash m_PluggedObjectsHash; ///< Association of base MAF3 objects and the list of plugged objects.
    mafCore::mafPluggedObjectsHash m_ReverseObjectsHash; ///< Hash needed for reverse association of plugged objects type and base class extended.
};

} // mafResources

#endif // MAFPLUGINMANAGER_H
