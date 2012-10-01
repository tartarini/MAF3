/*
 *  mafPlugin.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPlugin.h"
#include <stdexcept>

using namespace mafCore;
using namespace mafResources;

mafPlugin::mafPlugin(const QString &pluginFilename, const QString code_location) : mafPluginInterface(pluginFilename, code_location), m_LibraryHandler(0),
                                                          m_PluginInfo(0),
                                                          m_RegisterPlugin(0) {
    // Try to load the Plug-in as a dynamic library
    m_LibraryHandler = new QLibrary(pluginFilename);
    if(!m_LibraryHandler->load()) {
        delete m_LibraryHandler;
        m_LibraryHandler = NULL;
        return;
    }

    // Locate the Plug-ins exported functions
    try {
        m_PluginInfo = reinterpret_cast<mafFnPluginInfo *>(m_LibraryHandler->resolve("pluginInfo"));
        m_RegisterPlugin = reinterpret_cast<mafFnRegisterPlugin *>(m_LibraryHandler->resolve("registerPlugin"));

        // If the functions aren't found, we're going to assume this is
        // a plain simple DLL and not one of our Plug-ins
        if(!m_PluginInfo || !m_RegisterPlugin) {
            QString err_msg(mafTr("'%1' is not a valid MAF3 Plug-in").arg(pluginFilename));
            qCritical() << err_msg;
            delete m_LibraryHandler;
            m_LibraryHandler = NULL;
            return;
        }

        // Initialize a new DLL reference counter
        m_RefCount = new size_t(1);
    } catch(...) {
        QString err_msg(mafTr("Unknown error occurred loading Plug-in '%1'").arg(pluginFilename));
        qCritical() << err_msg;
        m_LibraryHandler->unload();
        delete m_LibraryHandler;
        m_LibraryHandler = NULL;
        return;
    }
    m_Loaded = true;
} 

mafPlugin::mafPlugin(const mafPlugin &Other, const QString code_location) : mafPluginInterface(Other, code_location), m_LibraryHandler(Other.m_LibraryHandler),
                                               m_PluginInfo(Other.m_PluginInfo),
                                               m_RegisterPlugin(Other.m_RegisterPlugin) {
    
}

mafPlugin::~mafPlugin() {
    // Only unload the DLL if there are no more references to it
    // Check also if m_RefCount is not NULL (it could be NULL when Plug-in loading error occur).
    if(m_RefCount && !--*m_RefCount) {
        delete m_RefCount;
        //m_LibraryHandler->unload();
        delete m_LibraryHandler;
        m_LibraryHandler = NULL;
    }
}

mafPluginInfo mafPlugin::pluginInfo() const {
    return m_PluginInfo();
}

void mafPlugin::registerPlugin() {
    m_RegisterPlugin();
}
