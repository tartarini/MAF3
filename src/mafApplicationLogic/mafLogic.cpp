/*
 *  mafLogic.cpp
 *  mafApplicationLogic
 *
 *  Created by Daniele Giunchi on 28/06/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafLogic.h"

#include <mafCoreSingletons.h>
#include <mafEventBusManager.h>
#include <mafMementoHierarchy.h>

using namespace mafEventBus;

#define PLUGIN_EXTENSION_FILTER "*.mafplugin"

#ifdef WIN32
    #define SHARED_OBJECT_PREFIX ""
    #define SHARED_OBJECT_SUFFIX ".dll"
#else
    #define SHARED_OBJECT_PREFIX "lib"
    #ifdef __APPLE__
        #define SHARED_OBJECT_SUFFIX ".dylib"
    #else
        #define SHARED_OBJECT_SUFFIX ".so"
    #endif
#endif




using namespace mafCore;
using namespace mafApplicationLogic;

mafLogic::mafLogic(const QString code_location) : mafLogicLight(code_location), m_WorkingDirectory(""), m_ApplicationDirectory(QDir::currentPath()) {
}

mafLogic::~mafLogic() {
    // Unload the mafResourcesLibrary.
    bool result(false);
    QHash<QString, QLibrary *>::const_iterator it = m_LibraryHandlersHash.constBegin();
    for (; it != m_LibraryHandlersHash.constEnd(); ++it) {
        result = mafShutdownModule(it.value());
    }
    m_LibraryHandlersHash.clear();
}

bool mafLogic::initialize() {
    bool result(Superclass::initialize());

    // Call the initialization of the superclass.
    if (result) {
        mafIdProvider *provider = mafIdProvider::instance();
        provider->createNewId("maf.local.logic.openFile");
        provider->createNewId("maf.local.logic.status.viewmanager.store");
        provider->createNewId("maf.local.logic.status.viewmanager.restore");
        provider->createNewId("maf.local.logic.status.vmemanager.store");
        provider->createNewId("maf.local.logic.status.vmemanager.restore");
        provider->createNewId("maf.local.logic.settings.store");
        provider->createNewId("maf.local.logic.settings.restore");

        // Signal registration.
        mafRegisterLocalSignal("maf.local.logic.openFile", this, "openFile(const QString)");
        mafRegisterLocalSignal("maf.local.logic.status.viewmanager.store", this, "statusViewManagerStore()");
        mafRegisterLocalSignal("maf.local.logic.status.viewmanager.restore", this, "statusViewManagerRestore(mafCore::mafMemento *, bool)");
        mafRegisterLocalSignal("maf.local.logic.status.vmemanager.store", this, "statusVmeManagerStore()");
        mafRegisterLocalSignal("maf.local.logic.status.vmemanager.restore", this, "statusVmeManagerRestore(mafCore::mafMemento *, bool)");
        mafRegisterLocalSignal("maf.local.logic.settings.store", this, "writeSettings()");
        mafRegisterLocalSignal("maf.local.logic.settings.restore", this, "readSettings()");

        // Slot registration.
        mafRegisterLocalCallback("maf.local.logic.settings.store", this, "storeSettings()");
        mafRegisterLocalCallback("maf.local.logic.settings.restore", this, "restoreSettings()");
        mafRegisterLocalCallback("maf.local.logic.openFile", this, "restoreHierarchy(QString)");

        mafRegisterLocalCallback("maf.local.resources.operation.executed", this, "executedOperation(QVariantMap )");
        
        m_CustomPluggedObjectsHash.clear();

        // Load the module related to the resources and managers and initialize it.
        QStringList sharedObjects;
        QString so;
        so.append(SHARED_OBJECT_PREFIX);
        so.append("mafResources");
        so.append(SHARED_OBJECT_SUFFIX);
        sharedObjects << so;
        so.clear();
        so.append(SHARED_OBJECT_PREFIX);
        so.append("mafSerialization");
        so.append(SHARED_OBJECT_SUFFIX);
        sharedObjects << so;

        QLibrary *handler(NULL);
        
        Q_FOREACH(so, sharedObjects) {
            handler = mafInitializeModule(so);
            if(handler) {
                m_LibraryHandlersHash.insert(so, handler);
            }
            result = result && (handler != NULL);
        }

        requestNewHierarchy();
    }

    // Perform design by contract check.
    ENSURE(result);
    return result;
}

mafCore::mafHierarchy *mafLogic::requestNewHierarchy() {
    // clean the SceneGraphs of all the views
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.clearViews");

    // Initialize data hierarchy
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafHierarchyPointer, m_Hierarchy);
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.new", mafEventTypeLocal, NULL, &ret_val);
    
    return m_Hierarchy;
}

void mafLogic::customizeVisualization(const QString view_name, const QString data_type, const QString pipe_visual_type) {
    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, view_name));
    argList.append(mafEventArgument(QString, data_type));
    argList.append(mafEventArgument(QString, pipe_visual_type));
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.customizeVisualization", mafEventTypeLocal, &argList);
}

void mafLogic::plugObject(const QString base_class, const QString class_type, const QString object_label) {
    // Add information to the
    mafPluggedObjectInformation objectInformation(object_label, class_type);
    // add the plugged object to the hash
    m_CustomPluggedObjectsHash.insertMulti(base_class, objectInformation);
}

void mafLogic::loadPlugins(QString plugin_dir) {
    // Compose the plugin absolute directory.
    QString pluginsDir = plugin_dir.isEmpty() ? (m_ApplicationDirectory + QDir::toNativeSeparators("/plugins/")) : plugin_dir;
    pluginsDir = QDir::cleanPath(pluginsDir);
    
    //check all plugin dirs
    QDir pdir(pluginsDir);
    pdir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot );
    QStringList plugin_dir_list = pdir.entryList();

    mafEventArgumentsList argList;
    Q_FOREACH(QString p, plugin_dir_list) {
        // Check for plugins to load
        QStringList filters;
        filters << PLUGIN_EXTENSION_FILTER;
        QString abs = pluginsDir;
        abs.append(QDir::toNativeSeparators("/" + p + "/"));
        QDir dir(abs);
        dir.setNameFilters(filters);
        dir.setFilter(QDir::Files);
        QStringList plugin_list = dir.entryList();
        
        qDebug() << plugin_list;
        
        // For each plugin file ask the plugin manager to load it through the event bus.
        Q_FOREACH(QString file, plugin_list) {
            argList.clear();
            file = dir.absoluteFilePath(file);
            QByteArray ba = file.toAscii();
            char *v = ba.data();
            argList.append(mafEventArgument(QString, file));
            mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.loadLibrary", mafEventTypeLocal, &argList);
        }
    }

    // Plug also the custom objects plugged from the vertical application.
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafPluggedObjectsHash, m_CustomPluggedObjectsHash));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.registerLibrary", mafEventTypeLocal, &argList);
}

void mafLogic::storeSettings() {
    qDebug() << "Writing mafLogic settings...";
    QSettings s;
    s.setValue("workingDir", m_WorkingDirectory);
}

void mafLogic::restoreSettings() {
    qDebug() << "Reading mafLogic settings...";
    QSettings s;
    m_WorkingDirectory = s.value("workingDir").toString();
}


void mafLogic::storeHierarchy(QString fileName, bool enableSaveAs) {
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(bool, enableSaveAs));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.changeIgnoreModified", mafEventTypeLocal, &argList);
    
    // Get hierarchy
    mafCore::mafHierarchyPointer hierarchy;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafHierarchyPointer, hierarchy);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.request", mafEventTypeLocal, NULL, &ret_val);
    
    mafMementoHierarchy * mementoHiearachy = (mafMementoHierarchy*)hierarchy->createMemento();
    REQUIRE(mementoHiearachy != NULL);

    // Serialize memento to file
    QString encodeType = "XML";
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafMemento *, mementoHiearachy));
    argList.append(mafEventArgument(QString, fileName));
    argList.append(mafEventArgument(QString, encodeType));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.save", mafEventTypeLocal, &argList);
    mafDEL(mementoHiearachy);
    
    // assign false to the ignore modified of the serialization manager
    argList.clear();
    argList.append(mafEventArgument(bool, false));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.changeIgnoreModified", mafEventTypeLocal, &argList);
    
}

void mafLogic::restoreHierarchy(QString fileName) {
    // Crate a new session.
    /// view select
    mafEventBusManager::instance()->notifyEvent("maf.local.gui.new", mafEventTypeLocal, NULL);

    //Load memento from file
    mafCore::mafMemento *mementoHierarchy = NULL;
    mafEventArgumentsList argList;
    QString encodeType = "XML";
    argList.append(mafEventArgument(QString, fileName));
    argList.append(mafEventArgument(QString, encodeType));
    QGenericReturnArgument retVal = mafEventReturnArgument(mafCore::mafMemento*, mementoHierarchy);
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.load", mafEventTypeLocal, &argList, &retVal);
    if(mementoHierarchy == NULL) {
        QByteArray ba = mafTr("Impossible to load MSF").toAscii();
        qCritical("%s", ba.data());
        return;
    }

    // Set memento loaded to hierarchy
    m_Hierarchy->setMemento(mementoHierarchy);
    mafDEL(mementoHierarchy);
    
    //fill the scene graphs of all the views
    // @@ TODO if hierarchy will handle all the hierarchy , and not shared the effort with all the other Objects, this method would be useless:
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.fillViews");
    
    //retrieve root
    mafObject *root;
    retVal = mafEventReturnArgument(mafCore::mafObject *, root);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.root", mafEventTypeLocal, NULL, &retVal);
    
    //select
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafObjectBase *, root));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.select", mafEventTypeLocal, &argList);

}

void mafLogic::executedOperation(QVariantMap response) {
    mafEventArgumentsList argList;
    argList.push_back(Q_ARG(QVariantMap, response));
        
    mafEvent dictionary;
    dictionary.setEventTopic("maf.local.eventBus.setResponseToNetwork");
    dictionary.setEventType(mafEventTypeLocal);
    mafEventBusManager::instance()->notifyEvent(dictionary, &argList);
}
