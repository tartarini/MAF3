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
 */
class MAFGUISHARED_EXPORT mafGUIManager : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafGUIManager(const mafString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafGUIManager();

    /// Create the actions associated with the menu items.
    virtual void createActions(QMainWindow *win);

    /// Create the menu for the vertical application.
    virtual void createMenus(QMainWindow *win);

    /// Create the toolbar for the vertical application and add them to the main window.
    virtual void createToolBars(QMainWindow *win);

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

private:
    /// Initialize the topics and register the signals with the mafEventBus.
    void registerSignals();

    QMenu *m_FileMenu; ///< Reference to the 'File' menu.
    QMenu *m_EditMenu; ///< Reference to the 'Edit' menu.
    QMenu *m_ViewMenu; ///< Reference to the 'View' menu.
    QMenu *m_OpMenu;   ///< Reference to the 'Operation' menu.
    QMenu *m_HelpMenu; ///< Reference to the 'Help' menu.

    QToolBar *m_FileToolBar; ///<
    QToolBar *m_EditToolBar; ///<

    QAction *m_NewAct; ///< Reference to the action associated to the 'New' Item creation.
    QAction *m_CollaborateAct; ///< Reference to the action associated to the 'Collaboration' chat Item.
    QAction *m_OpenAct; ///< Reference to the action associated to the 'Open' existing data.
    QAction *m_SaveAct; ///< Reference to the action associated to the 'Save' Items in a persistent way.
    QAction *m_SaveAsAct; ///< Reference to the action associated to the 'Save As' Items in a persistent way by assigning a name.
    QAction *m_ExitAct; ///< Reference to the action associated to the 'Exit'.

    QAction *m_CutAct;  ///< Reference to the action associated to the 'Cut' of Items from actual loaded data.
    QAction *m_CopyAct; ///< Reference to the action associated to the 'Copy' of Items from actual loaded data.
    QAction *m_PasteAct; ///< Reference to the action associated to the 'Paste' of Items into actual loaded data.

    QAction *m_AboutAct; ///< Reference to the action associated to the 'About' informative dialog.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline QMenu *mafGUIManager::fileMenu() const {
    return m_FileMenu;
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
