/*
 *  mafPluginInterface.h
 *  mafResources
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 21/03/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGININTERFACE_H
#define MAFPLUGININTERFACE_H

// Includes list
#include "mafResourcesDefinitions.h"

namespace mafResources {

/**
Class name: mafPluginInterface
This class provides the base class for plug-ins strategy.
*/
class MAFRESOURCESSHARED_EXPORT mafPluginInterface : public mafCore::mafObjectBase {
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object Constructor
    /** Loads the specified plugin as a dynamic library and locates
    the plugin's exported functions sFilename Filename of the plugin to load*/
    mafPluginInterface(const QString &sFilename, const QString code_location = "");

    /// Object Constructor
    /** Creates a copy of a plugin that has already been loaded.
    Required to provide correct semantics for storing plugins in
    an plugin map container.*/
    mafPluginInterface(const mafPluginInterface &Other, const QString code_location = "");

    /// Query the plugin for its expected engine version
    virtual mafPluginInfo pluginInfo() const = 0;

    /// Register the plug-in to a kernel.
    virtual void registerPlugin() = 0;

    /// Return true if plug in has been loaded.
    bool loaded();

protected:
    /// Destroys the plugin, unloading its library when no more references to it exist.
    /*virtual*/ ~mafPluginInterface();

    bool    m_Loaded; ///< True if plug in has been correctly loaded.
    size_t *m_RefCount;   ///< Number of references to the dynamic library
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline bool mafPluginInterface::loaded() {
  return m_Loaded;
}


} // mafResources

#endif // MAFPLUGININTERFACE_H
