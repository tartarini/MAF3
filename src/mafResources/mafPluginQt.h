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
#include "mafPluginQtInterface.h"
#include "mafPluginInterface.h"

namespace mafResources {

/**
Class name: mafPluginQt
This class provides the way MAF3 extend its objects through plug-ins.
*/
class MAFRESOURCESSHARED_EXPORT mafPluginQt : public mafPluginInterface {
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPluginInterface);

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
    /*virtual*/ mafPluginInfo pluginInfo() const;

    /// Register the plug-in to a kernel.
    /*virtual*/ void registerPlugin();

protected:
    /// Destroys the plugin, unloading its library when no more references to it exist.
    /*virtual*/ ~mafPluginQt();

private:
    mafPluginQtInterface *m_PluginRegistrator; ///< Qt Plugin Library handle
};

} // mafResources

#endif // MAFPLUGINQT_H
