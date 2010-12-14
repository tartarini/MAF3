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
#include <mafObjectBase.h>

#include <QtGui>

namespace mafGUI {

// Class forwarding list

/**
 Class name: mafGUIManager
 This is represent the manager for the GUI layer.
 The class will allow the creation of the basic elements composing the vertical application
 like the menu, the toolbar and the actions linked to them.
 The methods are virtuals and can be easyly redefined to create brand new toolbar or menu
 or extend the existing one by introducing new items.
 The manager defines also the topics related to the actions linked to the menubar and toolbar:
 - maf.local.gui.action.new
 - maf.local.gui.action.open
 - maf.local.gui.action.save
 - maf.local.gui.action.saveas
 - maf.local.gui.action.collaborate
 - maf.local.gui.action.cut
 - maf.local.gui.action.copy
 - maf.local.gui.action.paste
 - maf.local.gui.action.about

 There is also a signal related to the notification of path selection through a dialog.
 - maf.local.gui.pathSelected
 */
class MAFGUISHARED_EXPORT mafGUIManager : public mafCore::mafObjectBase {
    Q_OBJECT
    Q_PROPERTY(int maxRecentFiles READ maxRecentFiles WRITE setMaxRecentFiles)
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafGUIManager(QMainWindow *main_win, const mafString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafGUIManager();

    /// Create the menu for the vertical application.
    virtual void createMenus();

    /// Create the toolbar for the vertical application and add them to the main window.
    virtual void createToolBars();

    /// Return the number of maximum recent files.
    int maxRecentFiles();

    /// Allows to assign the maximum number of recent files. Canges of this number will be considered at the application's sturtup.
    void setMaxRecentFiles(int max_recent_files);

    /// Return the 'file' menu item created into the createMenus method.
    QMenu *fileMenu() const;

    /// Return the 'edit' menu item created into the createMenus method.
    QMenu *editMenu() const;

    /// Return the 'view' menu item created into the createMenus method.
    QMenu *viewMenu() const;

    /// Return the 'operation' menu item created into the createMenus method.
    QMenu *operationMenu() const;

    /// Return the 'help' menu item created into the createMenus method.
    QMenu *helpMenu() const;

    /// Show the file dialog to select the file path given a window title, a starting directory and an optional file wildcard
    void chooseFileDialog(const mafString title, const mafString start_dir, const mafString wildcard = mafTr("All files (*.*)"));

protected:
    /// Create the actions associated with the menu items.
    virtual void createActions();

public slots:
    /// Fill the operation and view menu with the plugged objects.
    void fillMenuWithPluggedObjects(mafCore::mafPluggedObjectsHash pluginHash);

signals:
    /// Signal emitted on path selection using the dialog.
    void pathSelected(const mafString path);

private slots:
    /// Start the operation associated with the operation's action activated.
    void startOperation();

    /// Create the view corresponding to the view's action activated.
    void createView();

    /// Allows to ask to open the selected recent file.
    void openRecentFile();

    /// Slot needed to intercept the started operation and ask it for the GUI filename.
    void operationDidStart(const mafCore::mafObjectBase *operation);

private:
    /// Initialize the topics and register the signals with the mafEventBus.
    void registerSignals();

    /// Update the recent file action with new recent file items.
    void updateRecentFileActions();

    /// Manage the filename of the recent file.
    mafString strippedName(const mafString &fullFileName);

    QMenu *m_FileMenu; ///< Reference to the 'File' menu.
    QMenu *m_RecentFilesMenu; ///< Reference to 'Recent File' menu.
    QMenu *m_EditMenu; ///< Reference to the 'Edit' menu.
    QMenu *m_ViewMenu; ///< Reference to the 'View' menu.
    QMenu *m_OpMenu;   ///< Reference to the 'Operation' menu.
    QMenu *m_HelpMenu; ///< Reference to the 'Help' menu.

    QToolBar *m_FileToolBar; ///< Toolbar related to File tasks
    QToolBar *m_EditToolBar; ///< Toolbar related to Edit tasks

    QAction *m_NewAct; ///< Reference to the action associated to the 'New' Item creation.
    QAction *m_CollaborateAct; ///< Reference to the action associated to the 'Collaboration' chat Item.
    QAction *m_OpenAct; ///< Reference to the action associated to the 'Open' existing data.
    QAction *m_SaveAct; ///< Reference to the action associated to the 'Save' Items in a persistent way.
    QAction *m_SaveAsAct; ///< Reference to the action associated to the 'Save As' Items in a persistent way by assigning a name.
    QAction *m_RecentFilesSeparatorAct; ///< Separator used to separate the recent files menu item from the previous one (if any recent is present)
    QAction *m_ExitAct; ///< Reference to the action associated to the 'Exit'.

    QAction *m_CutAct;  ///< Reference to the action associated to the 'Cut' of Items from actual loaded data.
    QAction *m_CopyAct; ///< Reference to the action associated to the 'Copy' of Items from actual loaded data.
    QAction *m_PasteAct; ///< Reference to the action associated to the 'Paste' of Items into actual loaded data.

    QAction *m_AboutAct; ///< Reference to the action associated to the 'About' informative dialog.

    int m_MaxRecentFiles; ///< Number of maximum recent files.
    mafList<QAction *> m_RecentFileActs; ///< List of recent file's actions.
    bool m_ActionsCreated; ///< Flag that store the actions' creation.

    QMainWindow *m_MainWindow; ///< Main window associated to the application.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline QMenu *mafGUIManager::fileMenu() const {
    return m_FileMenu;
}

inline void mafGUIManager::setMaxRecentFiles(int max_recent_files) {
    m_MaxRecentFiles = max_recent_files;
}

inline int mafGUIManager::maxRecentFiles() {
    return m_MaxRecentFiles;
}

inline QMenu *mafGUIManager::editMenu() const {
    return m_EditMenu;
}

inline QMenu *mafGUIManager::viewMenu() const {
    return m_ViewMenu;
}

inline QMenu *mafGUIManager::operationMenu() const {
    return m_OpMenu;
}

inline QMenu *mafGUIManager::helpMenu() const {
    return m_HelpMenu;
}

} // namespace mafGUI

#endif // MAFGUIMANAGER_H
