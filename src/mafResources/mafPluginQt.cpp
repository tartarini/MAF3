/*
 *  mafPluginQt.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPluginQt.h"
#include <stdexcept>

using namespace mafCore;
using namespace mafResources;

mafPluginQt::mafPluginQt(const QString &pluginFilename, const QString code_location) : mafPluginInterface(pluginFilename, code_location), m_PluginRegistrator(0) {
    // Try to load the plugin as a dynamic library
    QPluginLoader pluginLoader(pluginFilename);
    QObject *pg = pluginLoader.instance();
    m_PluginRegistrator = qobject_cast<mafPluginQtInterface *>(pg);
    m_Loaded = m_PluginRegistrator != NULL;
    if(!m_Loaded) {
        QString err_msg(mafTr("Could not load '%1'").arg(pluginFilename));
        qCritical() << err_msg;
        return;
    }

    m_RefCount = new size_t(1);
} 

mafPluginQt::mafPluginQt(const mafPluginQt &Other, const QString code_location) : mafPluginInterface(Other, code_location), m_PluginRegistrator(Other.m_PluginRegistrator) {
}

mafPluginQt::~mafPluginQt() {
    // Only unload the DLL if there are no more references to it
    // Check also if m_RefCount is not NULL (it could be NULL when plugin loading error occourr).
    if(m_RefCount && !--*m_RefCount) {
        delete m_RefCount;
        delete m_PluginRegistrator;
        m_PluginRegistrator = NULL;
    }
}

mafPluginInfo mafPluginQt::pluginInfo() const {
    if(m_PluginRegistrator) {
        return m_PluginRegistrator->pluginInfo();
    }
    
    qCritical() << mafTr("Info not loaded");
    return mafPluginInfo();
    
}
void mafPluginQt::registerPlugin() {
    if(m_PluginRegistrator) {
        m_PluginRegistrator->registerObjects();
    }
}

