/*
 *  mafGUIManager.h
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFGUIMANAGER_H
#define MAFGUIMANAGER_H

// Includes list
#include "mafGUIDefinitions.h"
#include <QtGui>
#include <QDomDocument>
#include <mafLogic.h>

// Class forwarding list
class mafOperationWidget;

namespace mafGUI {

// Class forwarding list
class mafUILoaderQt;
class mafTreeWidget;
class mafTreeModel;
class mafLoggerWidget;
class mafTextEditWidget;
class mafGUIApplicationSettingsDialog;
class mafGUIApplicationSettingsPage;

/**
 Class name: mafGUIManager
 This is represent the manager for the GUI layer.
 The class will allow the creation of the basic elements composing the vertical application
 like the menu, the toolbar and the actions linked to them.
 The methods are virtual and can be easily redefined to create brand new toolbar or menu
 or extend the existing one by introducing new items.
 The manager defines also the topics related to the actions linked to the menu bar and toolbar:
 - maf.local.gui.action.new
 - maf.local.gui.action.open
 - maf.local.gui.action.save
 - maf.local.gui.action.saveas
 - maf.local.gui.action.cut
 - maf.local.gui.action.copy
 - maf.local.gui.action.paste
 - maf.local.gui.action.about
 */
class MAFGUISHARED_EXPORT mafGUIManager : public mafCore::mafObjectBase {
    Q_OBJECT
    Q_PROPERTY(int maxRecentFiles READ maxRecentFiles WRITE setMaxRecentFiles)
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafGUIManager(QMainWindow *main_win, const QString code_location = "");

    /// Assign the application's logic.
    void setLogic(mafApplicationLogic::mafLogic *logic);

    /// Create the menu for the vertical application.
    virtual void createMenus();

    /// Create the toolbar for the vertical application and add them to the main window.
    virtual void createDefaultToolbars();

    /// Create the tree widget according to the given model passed as argument.
    virtual mafTreeWidget *createTreeWidget(mafTreeModel *model, QWidget *parent = 0);

    /// Create the log widget according to the given highlighter passed as argument.
    virtual mafTextEditWidget *createLogWidget(QWidget *parent = 0);

    /// Return the number of maximum recent files.
    int maxRecentFiles();

    /// Allows to assign the maximum number of recent files. Changes of this number will be considered at the application's startup.
    void setMaxRecentFiles(int max_recent_files);

    /// Return the application's settings dialog.
    mafGUIApplicationSettingsDialog *settingsDialog() const;

    /// Show the file dialog to select the file path given a window title, a starting directory and an optional file wildcard
    void chooseFileDialog(const QString title, const QString start_dir, const QString wildcard = mafTr("All files (*.*)"));

protected:
    /// Object destructor.
    /* virtual */ ~mafGUIManager();

    /// Create the actions associated with the menu items.
    virtual void createActions();

private:
    /// Initialize the topics and register the signals with the mafEventBus.
    void registerDefaultEvents();

    /// Update the recent file action with new recent file items.
    void updateRecentFileActions();

    /// Allow to parse XML attributes associated with a menu item or action.
    void parseMenuAttributes(QDomNode current);

    /// Allow to parse the XML tree hierarchy associated with the menu structure read from file.
    QDomNode parseMenuTree(QDomNode current);

    /// Create the action associated to the current menu item.
    void createAction(QDomElement node);

    /// Create the menu associated to the Dom node parsed.
    void createMenuItem(QDomElement node);

    /// Create the toolbar associated to the Dom node parsed.
    void createToolbar(QDomElement node);

    /// Create the default menu for the application in case that the mnu file has not been found.
    void createDefaultMenus();

    /// Manage the filename of the recent file.
    QString strippedName(const QString &fullFileName);

    QMenu *m_RecentFilesMenu; ///< Reference to 'Recent File' menu.
    QMenu *m_CurrentMenu; ///< Current menu that has been created during the mnu file parsing.

    QList<QObject *> m_MenuItemList; ///< List of created actions/menus.

    QAction *m_RecentFilesSeparatorAct; ///< Separator used to separate the recent files menu item from the previous one (if any recent is present)
    int m_MaxRecentFiles; ///< Number of maximum recent files.
    QList<QAction *> m_RecentFileActs; ///< List of recent file's actions.

    mafGUIApplicationSettingsDialog *m_SettingsDialog; ///< Settings dialog
    mafLoggerWidget *m_Logger; ///< Logger

    mafOperationWidget  *m_OperationWidget; ///< Widget on whith will be visible the operation's GUI.
    QWidget             *m_VMEWidget; ///< Widget representing the VME UI.
    QWidget             *m_ViewWidget; ///< Widget representing the View UI.
    QWidget             *m_VisualPipeWidget; ///< Widget representing the View UI.

    mafObjectBase *m_CurrentPipeVisual; ///< Current used pipe visual.
    mafObjectBase *m_CurrentView; ///< Current view.

    QMainWindow     *m_MainWindow;  ///< Main window associated to the application.
    mafUILoaderQt   *m_UILoader;    ///< Class in charge to load the GUI.
    mafGUILoadedType m_GUILoadedType; ///< Type of GUI loaded.
    mafTreeModel    *m_Model;       ///< Tree model of VME.
    mafTreeWidget   *m_TreeWidget;  ///< Visualize the tree model.
    mafApplicationLogic::mafLogic *m_Logic; ///< Logic of the application.

signals:
    /// Signal emitted when the GUI panel has been loaded.
    void guiLoaded(int type, QWidget *w);

    /// Signal emitted when a dynamic GUI element needs to be removed from the MainWindow
    void guiTypeToRemove(int type);

    /// Signal emitted when a new hierarchy is requested on open a msf file.
    void newWorkinSessioneSignal();

    /// Signal used to ask for updating of application name.
    void updateApplicationName();

public slots:
    /// Fill the operation and view menu with the plugged objects.
    void fillMenuWithPluggedObjects(mafCore::mafPluggedObjectsHash pluginHash);

    /// Return the Action/Menu associated with the given name.
    QObject *menuItemByName(QString name);

    /// Slot connected with the New Action.
    virtual void newWorkingSession();

    /// Slot called by the Quit Action.
    virtual void quitApplication();

    /// Return name of working file.
    QString fileName();

private slots:
    /// Start the operation associated with the operation's action activated.
    void startOperation();

    /// Remove the Operation's GUI; the 'Ok' or 'Cancel' button has been pressed.
    void removeOperationGUI();

    /// Create the view corresponding to the view's action activated.
    void createView();

    /// Update the GUI associated to the selected view.
    void viewSelected(mafCore::mafObjectBase *view);

    /// Set mafVMEManager hierarchy to the tree model.
    void viewDestroyed();

    /// Allow to send a vme selection request when an item has been clicked into the mafTreWidget.
    void selectVME(QModelIndex index);

    /// Allows to ask to open the selected recent file.
    void openRecentFile();

    /// Show the application's settings dialog.
    void showSettingsDialog();

    /// Called when a VME has been selected.
    /** This method will update all the operation's menu items according to the new selected VME.*/
    void updateMenuForSelectedVme(mafCore::mafObjectBase *vme);

    /// Called when a VME has been selected.
    /** This method will update all tree widget for the selected VME */
    void updateTreeForSelectedVme(mafCore::mafObjectBase *vme);

    /// Called when a scene node has been selected.
    /** This method will update gui for the selected scene node */
    void updateGuiForSelectedPipeVisual(mafCore::mafObjectBase *pipeVisual);
    
    /// Slot needed to intercept the started operation and ask it for the GUI filename.
    void operationDidStart(mafCore::mafObjectBase *operation);

    /// Slot called when the UI is loaded from the mafUILoaderQt, and the gui is shown.
    void showGui(mafCore::mafProxyInterface *guiWidget);
    
    /// save the current working session.
    void save();

    /// save as the current working session.
    void saveAs();
    
    /// open a working session
    void open();

private:
    /// return the data object inside an item in order to retrieve the data object and not the visual one (for example scene-node).
    QObject *dataObject(QModelIndex index);

    /// Update recent file menu
    void updateRecentFileMenu(QString fileName);

    QString m_CompleteFileName; ///< Name of the msf file.
    QString m_LastPath; ///< Path of last msf file.

};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafGUIManager::setMaxRecentFiles(int max_recent_files) {
    m_MaxRecentFiles = max_recent_files;
}

inline int mafGUIManager::maxRecentFiles() {
    return m_MaxRecentFiles;
}

inline mafGUIApplicationSettingsDialog *mafGUIManager::settingsDialog() const {
    return m_SettingsDialog;
}

inline void mafGUIManager::setLogic(mafApplicationLogic::mafLogic *logic) {
    m_Logic = logic;
}

inline QString mafGUIManager::fileName() {
    return m_CompleteFileName;
}

} // namespace mafGUI

#endif // MAFGUIMANAGER_H
