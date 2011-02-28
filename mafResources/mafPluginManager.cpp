/*
 *  mafPluginManager.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPluginManager.h"
#include "mafPlugin.h"

using namespace mafCore;
using namespace mafResources;

mafPluginManager* mafPluginManager::instance() {
    // Create the instance of the plugin manager.
    static mafPluginManager instancePluginManager;
    return &instancePluginManager;
}

void mafPluginManager::shutdown() {
}

mafPluginManager::mafPluginManager(const QString code_location) : mafObjectBase(code_location) {
    // Create the Topic needed to load the external library containing the plug-ins.
    mafId load_library_id = mafIdProvider::instance()->createNewId("maf.local.resources.plugin.loadLibrary");
    if(load_library_id != -1) {
        mafRegisterLocalSignal("maf.local.resources.plugin.loadLibrary", this, "loadPluginLibrary(const QString &)");
        mafRegisterLocalCallback("maf.local.resources.plugin.loadLibrary", this, "loadPlugin(const QString &)");
    }

    // Create the Topic needed to register plug-in objects.
    mafId register_plugin_id = mafIdProvider::instance()->createNewId("maf.local.resources.plugin.registerLibrary");
    if(register_plugin_id != -1) {
        mafRegisterLocalSignal("maf.local.resources.plugin.registerLibrary", this, "registerPluginToManager(mafCore::mafPluggedObjectsHash)");
        mafRegisterLocalCallback("maf.local.resources.plugin.registerLibrary", this, "registerPlugin(mafCore::mafPluggedObjectsHash)");
    }

    // Create the Topic to perform a query on the plugged libraries.
    mafId query_plugin_id = mafIdProvider::instance()->createNewId("maf.local.resources.plugin.resourcesQuery");
    if(query_plugin_id != -1) {
        mafRegisterLocalSignal("maf.local.resources.plugin.resourcesQuery", this, "queryPluggedObjectsSignal(const QString &)");
        mafRegisterLocalCallback("maf.local.resources.plugin.resourcesQuery", this, "queryPluggedObjects(const QString &)");
    }
}

mafPluginManager::~mafPluginManager() {
    // Delete all references of plug-ins.
    mafPluginHash::iterator iter = m_PluginsHash.begin();
    while(iter != m_PluginsHash.end()) {
        mafPlugin *p = iter.value();
        mafDEL(p);
        ++iter;
    }
    m_PluginsHash.clear();
}

void mafPluginManager::loadPlugin( const QString &pluginFilename ) {
    if(!m_PluginsHash.contains(pluginFilename))
        m_PluginsHash.insert(pluginFilename, new mafPlugin(pluginFilename, mafCodeLocation)).value()->registerPlugin();
}

//void mafPluginManager::unLoadPlugin(const QString &pluginFilename) {
//    mafPlugin *p = m_PluginsHash.value(pluginFilename);
//    mafDEL(p);
//    m_PluginsHash.remove(pluginFilename);
//}

mafPluginInfo mafPluginManager::pluginInformation(QString plugin_name) {
    mafPlugin *p = m_PluginsHash.value(plugin_name);
    return p->pluginInfo();
}

//void mafPluginManager::registerPlugin(const QString &baseClassExtended, const QString &pluggedObjectType, const QString &objectLabel) {
void mafPluginManager::registerPlugin(mafCore::mafPluggedObjectsHash pluginHash) {
    // Store plugin information to be queryed starting from base MAF class extended.
    // This is useful to show for example all the plugged mafView in the main menu.

    QString base_class("");
    mafPluggedObjectInformation objInfo;
    mafPluggedObjectsHash::iterator iter = pluginHash.begin();
    while(iter != pluginHash.end()) {
        objInfo = iter.value();
        base_class = iter.key();
        if(base_class.isEmpty()) {
            qWarning("%s", mafTr("Try to plug %1 that has no base class reference!!").arg(objInfo.m_ClassType).toAscii().data());
        } else {
            m_PluggedObjectsHash.insertMulti(base_class, objInfo);

            // Store the same information but in reverse order needed for example to retrieve the base MAF class type from the given plugged object type.
            // This is useful for example to retrieve the list of visual pipe that accept an object.
            mafPluggedObjectInformation reverseObjectInfo(objInfo.m_Label, base_class);
            m_ReverseObjectsHash.insert(objInfo.m_ClassType, reverseObjectInfo);
        }
        ++iter;
    }
}

mafPluggedObjectInformationList *mafPluginManager::queryPluggedObjects(const QString &baseMAFClassExtended) {
    REQUIRE(baseMAFClassExtended.length() > 0);

    mafPluggedObjectInformationList *resultList = new mafPluggedObjectInformationList();
    resultList->append(m_PluggedObjectsHash.values(baseMAFClassExtended));
    return resultList;
}

void mafPluginManager::queryBaseClassType(QString &pluggedObjectClassType, mafPluggedObjectInformationList *resultBaseClass) {
    REQUIRE(resultBaseClass != NULL);

    mafPluggedObjectInformation result = m_ReverseObjectsHash.value(pluggedObjectClassType);
    resultBaseClass->append(result);
}
