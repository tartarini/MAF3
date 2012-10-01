/*
 *  mafPlugin.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGIN_H
#define MAFPLUGIN_H

// Includes list
#include "mafResourcesDefinitions.h"
#include "mafPluginInterface.h"

namespace mafResources {

/**
Class name: mafPlugin
This class provides the way MAF3 extend its objects through plug-ins.
*/
class MAFRESOURCESSHARED_EXPORT mafPlugin : public mafPluginInterface {
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPluginInterface);

public:
    /// Object Constructor
    /** Loads the specified plugin as a dynamic library and locates
    the plugin's exported functions sFilename Filename of the plugin to load*/
    mafPlugin(const QString &sFilename, const QString code_location = "");

    /// Object Constructor
    /** Creates a copy of a plugin that has already been loaded.
    Required to provide correct semantics for storing plugins in
    an plugin map container.*/
    mafPlugin(const mafPlugin &Other, const QString code_location = "");

    /// Query the plugin for its expected engine version
    /*virtual*/ mafPluginInfo pluginInfo() const;

    /// Register the plug-in to a kernel.
    /*virtual*/ void registerPlugin();
    
protected:
    /// Destroys the plugin, unloading its library when no more references to it exist.
    /*virtual*/ ~mafPlugin();

private:
    /// Signature for the version query function
    typedef mafPluginInfo mafFnPluginInfo();

    /// Signature for the plugin's registration function
    typedef void mafFnRegisterPlugin();

    QLibrary            *m_LibraryHandler; ///< Plugin Library handle
    mafFnPluginInfo     *m_PluginInfo;     ///< Plug-in information query function
    mafFnRegisterPlugin *m_RegisterPlugin; ///< Plugin registration function
};

} // mafResources

#endif // MAFPLUGIN_H
