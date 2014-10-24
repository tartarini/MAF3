/*
 *  mafLogic.h
 *  mafApplicationLogic
 *
 *  Created by Daniele Giunchi on 28/06/10.
 *  Copyright 2009-2010 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFLOGIC_H
#define MAFLOGIC_H

// Includes list
#include "mafLogicLight.h"
#include <mafMemento.h>
#include <mafHierarchy.h>

#include <QDir>

namespace mafApplicationLogic {

// Class forwarding list

/**
 Class name: mafLogic
 This class define the full implementation of the framework logic.
 It allows to instantiate all the managers, and use initialization of mafLogicLight.
 This logic class can be used with those applications that needs of a complete version of the framework
 using plugin and resources.
 mafLogic defines also these topics:
  - maf.local.logic.openFile that allow to notify the request to open the given filename passed ar argument.
  - maf.local.logic.settings.store allow to start the application's settings store.
  - maf.local.logic.settings.restore allow to start the application's settings restore.
  - maf.local.logic.settings.gui.store that allows to ask the mafGUIManager to store its settings.
  - maf.local.logic.settings.gui.restore that allows to ask the mafGUIManager to restore a previous saved state of its settings.
  - maf.local.logic.status.viewmanager.store that allows to ask the mafViewManager to store its status into a memento that will be serialized.
  - maf.local.logic.status.viewmanager.restore that allows to ask the mafViewManager to restore a previous saved state.
  - maf.local.logic.status.vmemanager.store that allows to ask the mafVmeManager to store its status into a memento that will be serialized.
  - maf.local.logic.status.vmemanager.restore that allows to ask the mafVmeManager to restore a previous saved state.
*/
class MAFAPPLICATIONLOGICSHARED_EXPORT mafLogic : public mafLogicLight {
    Q_OBJECT
    Q_PROPERTY(QString workingDir READ workingDirectory WRITE setWorkingDirectory);
    Q_PROPERTY(QString applicationName READ applicationName WRITE setApplicationName);
    /// typedef macro.
    mafSuperclassMacro(mafLogic::mafLogicLight);

public Q_SLOTS:
    /// Allows to assign the application's working directory.
    void setWorkingDirectory(const QString wd);

    /// Orchestrate the requests for the store the application's settings.
    void storeSettings();

    /// Orchestrate the requests for restoring the application's settings.
    void restoreSettings();

    /// Create a new VME Hierarchy.
    mafCore::mafHierarchy *requestNewHierarchy();

    /// Save hierarchy in a memento
    void storeHierarchy(QString fileName, bool enableSaveAs = false);

    /// Load hierarchy from a memento.
    void restoreHierarchy(QString fileName);

private Q_SLOTS:
    /// manage the response of an operation, after its execution.
    void executedOperation(QVariantMap response);
    
Q_SIGNALS:
    /// Signal emitted when a recent file is asked to be opened. The argument of the signal contains the filename.
    void openFile(const QString filename);

    /// Signal used to alert all observers to write the owned settings.
    void writeSettings();

    /// Signal used to alert all observer to read back saved settings.
    void readSettings();

    /// Signal used to ask the view manager to store its status.
    void statusViewManagerStore();

    /// Signal used to ask the view manager to restore its status.
    void statusViewManagerRestore(mafCore::mafMemento *memento, bool deep);

    /// Signal used to ask the vme manager to store its status.
    void statusVmeManagerStore();

    /// Signal used to ask the vme manager to restore its status.
    void statusVmeManagerRestore(mafCore::mafMemento *memento, bool deep);
                  
public:
    /// Object constructor.
    mafLogic(const QString code_location = "");

    /// Allows to initialize all the singletons and register all the objects to the factory.
    /// Set light to true if mafResource and mafSerialization module are not required by the application.
    virtual bool initialize();

    /// Allows to retrieve the application's working directory.
    QString workingDirectory() const;
    
    /// Allows to retrieve the application's name.
    QString applicationName() const;
    
    /// Assign the application's name
    void setApplicationName(QString name);

    /// Returns the VME Hierarchy.
    mafCore::mafHierarchy *hierarchy();

    /// Allow to load plugins located into the 'plugins' folder located into the application's directory path.
    /** The optional argument allows logic to load a plugin from a given directory. By default the
    plugin directory will be ApplicationDirectory/plugins.*/
    void loadPlugins(QString plugin_dir = "");

    /// Method used to add custom object defined in vertical application.
    /** This type of addition to the framework is the same of that one done by the plug-in
    but is used to add a small number of objects without encapsulating them into a plug-in,
    because for example are still in beta testing phase or because is needed to plug only one
    custom class.*/
    void plugObject(const QString base_class, const QString class_type, const QString object_label);

    /// Plug the information to customize the visualization for a VME shown into the given view with the associated pipe visual.
    void customizeVisualization(const QString view_name, const QString data_type, const QString pipe_visual_type);

    /// Plug the information to customize the widget associated to a specific view.
    void customizeViewWidget(const QString view_name, const QString rootWidget_name);
    
    /// Push a library name in the list of libraries to load in app initialization.
    void pushLibraryToLoad(QString libraryName);
                  
protected:
    /// Object destructor.
    /*virtual*/ ~mafLogic();

private:
    QString m_WorkingDirectory; ///< Contains the root directory of the application's data.
    QString m_ApplicationName; ///< Contains the application's name.
    mafCore::mafPluggedObjectsHash m_CustomPluggedObjectsHash; ///< Hash containing operations, views and all the plugged custom objects.
    QStringList m_LibrariesList; ///< contains the name of the libraries which needs to be loaded in app initialization.
    QHash<QString, QLibrary *> m_LibraryHandlersHash; ///< Hash containing  the loaded plugins.
    mafCore::mafHierarchy *m_Hierarchy; ///< VME Hierarchy
};

inline void mafLogic::setWorkingDirectory(const QString wd) {
    m_WorkingDirectory =  QDir::toNativeSeparators(wd);

#if !defined(_WIN32) && !defined(WIN32)
    m_WorkingDirectory.replace('\\','/');
#endif
}

inline QString mafLogic::workingDirectory() const {
    return m_WorkingDirectory;
}
    
inline QString mafLogic::applicationName() const {
    return m_ApplicationName;
}
    
inline void mafLogic::setApplicationName(QString name) {
    m_ApplicationName = name;
}

inline mafCore::mafHierarchy *mafLogic::hierarchy() {
    return m_Hierarchy;
}

} // namespace mafApplicationLogic

#endif // MAFLOGIC_H
