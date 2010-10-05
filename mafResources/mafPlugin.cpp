/*
 *  mafPlugin.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPlugin.h"
#include <stdexcept>

using namespace mafCore;
using namespace mafResources;

mafPlugin::mafPlugin(const mafString &pluginFilename, const mafString code_location) : mafObjectBase(code_location), m_LibraryHandler(0),
                                                          m_RefCount(0),
                                                          m_PluginInfo(0),
                                                          m_RegisterPlugin(0)  {
    // Try to load the plugin as a dynamic library
    m_LibraryHandler = new mafLibrary(pluginFilename);
    if(!m_LibraryHandler->load()) {
        mafString err_msg(mafTr("Could not load '%1'").arg(pluginFilename));
        throw std::runtime_error(err_msg.toAscii().constData());
    }

    // Locate the plugin's exported functions
    try {
        m_PluginInfo = reinterpret_cast<mafFnPluginInfo *>(m_LibraryHandler->resolve("pluginInfo"));
        m_RegisterPlugin = reinterpret_cast<mafFnRegisterPlugin *>(m_LibraryHandler->resolve("registerPlugin"));

        // If the functions aren't found, we're going to assume this is
        // a plain simple DLL and not one of our plugins
        if(!m_PluginInfo || !m_RegisterPlugin) {
            mafString err_msg(mafTr("'%1' is not a valid MAF3 plugin").arg(pluginFilename));
            throw std::runtime_error(err_msg.toAscii().constData());
        }

        // Initialize a new DLL reference counter
        m_RefCount = new size_t(1);
    } catch(...) {
        m_LibraryHandler->unload();
        mafDEL(m_LibraryHandler);
        throw;
    }
} 

mafPlugin::mafPlugin(const mafPlugin &Other, const mafString code_location) : mafObjectBase(code_location), m_LibraryHandler(Other.m_LibraryHandler),
                                               m_RefCount(Other.m_RefCount) ,
                                               m_PluginInfo(Other.m_PluginInfo),
                                               m_RegisterPlugin(Other.m_RegisterPlugin) {
    ++*m_RefCount; // Increase DLL reference counter
}

mafPlugin::~mafPlugin() {
    // Only unload the DLL if there are no more references to it
    if(!--*m_RefCount) {
        delete m_RefCount;
        //m_LibraryHandler->unload();
        mafDEL(m_LibraryHandler);
    }
}
