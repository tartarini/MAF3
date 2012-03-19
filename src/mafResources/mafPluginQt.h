/*
 *  mafPluginQt.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINQT_H
#define MAFPLUGINQT_H

// Includes list
#include "mafResourcesDefinitions.h"

namespace mafResources {

// Class forwarding list
class mafPluginManager;
class mafPluginQtInterface;

/**
Class name: mafPluginQt
This class provides the way MAF3 extend its objects through plug-ins.
*/
class MAFRESOURCESSHARED_EXPORT mafPluginQt : public mafCore::mafObjectBase {
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object Constructor
    /** Loads the specified plugin as a dynamic library and locates
    the plugin's exported functions sFilename Filename of the plugin to load*/
    mafPluginQt(const QString &sFilename, const QString code_location = "");

    /// Object Constructor
    /** Creates a copy of a plugin that has already been loaded.
    Required to provide correct semantics for storing plugins in
    an plugin map container.*/
    mafPluginQt(const mafPluginQt &Other, const QString code_location = "");

    /// Query the plugin for its expected engine version
    mafPluginInfo pluginInfo() const;

    /// Register the plug-in to a kernel.
    void registerPlugin();

    /// Return true if plug in has been loaded.
    bool loaded();

protected:
    /// Destroys the plugin, unloading its library when no more references to it exist.
    /*virtual*/ ~mafPluginQt();

private:
    mafPluginQtInterface *m_PluginRegistrator; ///< Qt Plugin Library handle
    size_t               *m_RefCount;   ///< Number of references to the dynamic library
    bool                  m_Loaded; ///< True if plug in has been correctly loaded.
};


inline bool mafPluginQt::loaded() {
  return m_Loaded;
}


} // mafResources

#endif // MAFPLUGINQT_H
