/*
 *  mafPlugin.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGIN_H
#define MAFPLUGIN_H

// Includes list
#include "mafResourcesDefinitions.h"

namespace mafResources {

// Class forwarding list
class mafPluginManager;

/**
Class name: mafPlugin
This class provides the way MAF3 extend its objects through plug-ins.
*/
class MAFRESOURCESSHARED_EXPORT mafPlugin : public mafCore::mafObjectBase {
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

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
    mafPluginInfo pluginInfo() const {
        return m_PluginInfo();
    }

    /// Register the plug-in to a kernel.
    void registerPlugin() {
        m_RegisterPlugin();
    }

    /// Return true if plug in has been loaded.
    bool loaded();

protected:
    /// Destroys the plugin, unloading its library when no more references to it exist.
    /*virtual*/ ~mafPlugin();

private:
    /// Signature for the version query function
    typedef mafPluginInfo mafFnPluginInfo();

    /// Signature for the plugin's registration function
    typedef void mafFnRegisterPlugin();

    QLibrary          *m_LibraryHandler; ///< Plugin Library handle
    size_t              *m_RefCount;   ///< Number of references to the dynamic library
    mafFnPluginInfo     *m_PluginInfo;  ///< Plug-in information query function
    mafFnRegisterPlugin *m_RegisterPlugin; ///< Plugin registration function
    bool                 m_Loaded; ///< True if plug in has been correctly loaded.
};


inline bool mafPlugin::loaded() {
  return m_Loaded;
}


} // mafResources

#endif // MAFPLUGIN_H
