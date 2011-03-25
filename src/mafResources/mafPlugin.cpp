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

mafPlugin::mafPlugin(const QString &pluginFilename, const QString code_location) : mafObjectBase(code_location), m_LibraryHandler(0),
                                                          m_RefCount(0),
                                                          m_PluginInfo(0),
                                                          m_RegisterPlugin(0)  {
    // Try to load the plugin as a dynamic library
    m_LibraryHandler = new QLibrary(pluginFilename);
    if(!m_LibraryHandler->load()) {
        QString err_msg(mafTr("Could not load '%1'").arg(pluginFilename));
        qCritical() << err_msg;
        delete m_LibraryHandler;
        m_LibraryHandler = NULL;
        return;
    }

    // Locate the plugin's exported functions
    try {
        m_PluginInfo = reinterpret_cast<mafFnPluginInfo *>(m_LibraryHandler->resolve("pluginInfo"));
        m_RegisterPlugin = reinterpret_cast<mafFnRegisterPlugin *>(m_LibraryHandler->resolve("registerPlugin"));

        // If the functions aren't found, we're going to assume this is
        // a plain simple DLL and not one of our plugins
        if(!m_PluginInfo || !m_RegisterPlugin) {
            QString err_msg(mafTr("'%1' is not a valid MAF3 plugin").arg(pluginFilename));
            qCritical() << err_msg;
            delete m_LibraryHandler;
            m_LibraryHandler = NULL;
            return;
        }

        // Initialize a new DLL reference counter
        m_RefCount = new size_t(1);
    } catch(...) {
        QString err_msg(mafTr("Unknoun error occourred loading plugin '%1'").arg(pluginFilename));
        qCritical() << err_msg;
        m_LibraryHandler->unload();
        delete m_LibraryHandler;
        m_LibraryHandler = NULL;
        return;
    }
} 

mafPlugin::mafPlugin(const mafPlugin &Other, const QString code_location) : mafObjectBase(code_location), m_LibraryHandler(Other.m_LibraryHandler),
                                               m_RefCount(Other.m_RefCount) ,
                                               m_PluginInfo(Other.m_PluginInfo),
                                               m_RegisterPlugin(Other.m_RegisterPlugin) {
    ++*m_RefCount; // Increase DLL reference counter
}

mafPlugin::~mafPlugin() {
    // Only unload the DLL if there are no more references to it
    // Check also if m_RefCount is not NULL (it could be NULL when plugin loading error occourr).
    if(m_RefCount && !--*m_RefCount) {
        delete m_RefCount;
        //m_LibraryHandler->unload();
        delete m_LibraryHandler;
        m_LibraryHandler = NULL;
    }
}
