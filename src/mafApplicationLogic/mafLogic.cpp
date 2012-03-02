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

#define PLUGIN_EXTENSION_FILTER "*.mafplugin"

#ifdef WIN32
    #define SHARED_OBJECT_PREFIX ""
    #define SHARED_OBJECT_SUFFIX ".dll"
#else
    #define SHARED_OBJECT_PREFIX "lib"
    #ifdef __APPLE__
        #define SHARED_OBJECT_PREFIX QString(m_ApplicationName).append("/Contents/MacOS/")
        #define SHARED_OBJECT_SUFFIX ".dylib"
    #else
        #define SHARED_OBJECT_SUFFIX ".so"
    #endif
#endif


using namespace mafCore;
using namespace mafEventBus;
using namespace mafApplicationLogic;

mafLogic::mafLogic(const QString code_location) : mafLogicLight(code_location), m_WorkingDirectory(""), m_ApplicationName(""), m_Hierarchy(NULL) {
    m_LibrariesList.clear();
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
        QLibrary *handler(NULL);
        Q_FOREACH(QString so, m_LibrariesList) {
            QString soLibName(SHARED_OBJECT_PREFIX);
            soLibName.append(so);
            soLibName.append(SHARED_OBJECT_SUFFIX);
            handler = mafInitializeModule(soLibName);
            if(handler) {
                m_LibraryHandlersHash.insert(soLibName, handler);
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
    mafEvent ev("maf.local.resources.hierarchy.new");
    ev.setReturnValue(mafEventReturnArgument(mafCore::mafHierarchyPointer, m_Hierarchy));

    mafEventBusManager::instance()->notifyEvent(ev);
   
    return m_Hierarchy;
}

void mafLogic::customizeVisualization(const QString view_name, const QString data_type, const QString pipe_visual_type) {
    mafEvent ev("maf.local.resources.view.customizeVisualization");
    ev.addParameter(mafEventArgument(QString, view_name));
    ev.addParameter(mafEventArgument(QString, data_type));
    ev.addParameter(mafEventArgument(QString, pipe_visual_type));
    mafEventBusManager::instance()->notifyEvent(ev);
}


void mafLogic::customizeViewWidget(const QString view_name, QString widget_name) {
    mafEvent ev("maf.local.resources.view.customizeViewWidget");
    ev.addParameter(mafEventArgument(QString, view_name));
    ev.addParameter(mafEventArgument(QString, widget_name));
    mafEventBusManager::instance()->notifyEvent(ev);
}

void mafLogic::plugObject(const QString base_class, const QString class_type, const QString object_label) {
    // Add information to the
    mafPluggedObjectInformation objectInformation(object_label, class_type);
    // add the plugged object to the hash
    m_CustomPluggedObjectsHash.insertMulti(base_class, objectInformation);
}

void mafLogic:: pushLibraryToLoad(QString libraryName) {
    m_LibrariesList << libraryName;
}

void mafLogic::loadPlugins(QString plugin_dir) {
    // Compose the plugin absolute directory.
    QString appDir(QCoreApplication::applicationDirPath());
    QString pluginsDir = plugin_dir.isEmpty() ? (appDir + QDir::toNativeSeparators("/plugins/")) : plugin_dir;
    pluginsDir = QDir::cleanPath(pluginsDir);
    
    //check all plugin dirs
    QDir pdir(pluginsDir);
    pdir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot );
    QStringList plugin_dir_list = pdir.entryList();

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
            file = dir.absoluteFilePath(file);
            QByteArray ba = file.toAscii();
            char *v = ba.data();
            mafEvent ev("maf.local.resources.plugin.loadLibrary");
            ev.addParameter(mafEventArgument(QString, file));
            mafEventBusManager::instance()->notifyEvent(ev);
        }
    }

    // Plug also the custom objects plugged from the vertical application.
    mafEvent ev("maf.local.resources.plugin.registerLibrary");
    ev.addParameter(mafEventArgument(mafCore::mafPluggedObjectsHash, m_CustomPluggedObjectsHash));
    mafEventBusManager::instance()->notifyEvent(ev);
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
    mafEvent ev("maf.local.serialization.changeIgnoreModified");
    ev.addParameter(mafEventArgument(bool, enableSaveAs));
    mafEventBusManager::instance()->notifyEvent(ev);
    
    // Get hierarchy
    mafCore::mafHierarchyPointer hierarchy;
    ev.clearArgList();
    ev[TOPIC] = "maf.local.resources.hierarchy.request";
    ev.setReturnValue(mafEventReturnArgument(mafCore::mafHierarchyPointer, hierarchy));
    mafEventBusManager::instance()->notifyEvent(ev);
    
    mafMemento * mementoHiearachy = hierarchy->createMemento();
    REQUIRE(mementoHiearachy != NULL);

    // Serialize memento to file
    QString encodeType = "XML";
    mafEvent evSave("maf.local.serialization.save");
    evSave.addParameter(mafEventArgument(mafCore::mafMemento *, mementoHiearachy));
    evSave.addParameter(mafEventArgument(QString, fileName));
    evSave.addParameter(mafEventArgument(QString, encodeType));
    mafEventBusManager::instance()->notifyEvent(evSave);
    mafDEL(mementoHiearachy);
    
    // assign false to the ignore modified of the serialization manager
    ev.clearArgList();
    ev[TOPIC] = "maf.local.serialization.changeIgnoreModified";
    ev.addParameter(mafEventArgument(bool, false));
    mafEventBusManager::instance()->notifyEvent(ev);
}

void mafLogic::restoreHierarchy(QString fileName) {
    //Load memento from file
    mafCore::mafMemento *mementoHierarchy = NULL;
    QString encodeType = "XML";

    mafEvent ev("maf.local.serialization.load");
    ev.addParameter(mafEventArgument(QString, fileName));
    ev.addParameter(mafEventArgument(QString, encodeType));
    ev.setReturnValue(mafEventReturnArgument(mafCore::mafMemento*, mementoHierarchy));
    mafEventBusManager::instance()->notifyEvent(ev);
    
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
    mafEvent evFillViews("maf.local.resources.view.fillViews");
    mafEventBus::mafEventBusManager::instance()->notifyEvent(evFillViews);
    
    //retrieve root
    mafObject *root;
    mafEvent evRoot("maf.local.resources.hierarchy.root");
    evRoot.setReturnValue(mafEventReturnArgument(mafCore::mafObject *, root));
    mafEventBusManager::instance()->notifyEvent(evRoot);
    
    //select
    mafEvent evSel("maf.local.resources.vme.select");
    evRoot.addParameter(mafEventArgument(mafCore::mafObjectBase *, root));
    mafEventBusManager::instance()->notifyEvent(evSel);
}

void mafLogic::executedOperation(QVariantMap response) {
    mafEvent dictionary("maf.local.eventBus.setResponseToNetwork");
    dictionary.addParameter(mafEventArgument(QVariantMap, response));
    mafEventBusManager::instance()->notifyEvent(dictionary);
}
