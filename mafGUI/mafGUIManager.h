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
#include "mafGUI_global.h"
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
    virtual void createActions();

    /// Create the menu for the vertical application.
    virtual void createMenus();

    /// Create the toolbar for the vertical application.
    virtual void createToolBars();

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
    QMenu *m_FileMenu;
    QMenu *m_EditMenu;
    QMenu *m_ViewMenu;
    QMenu *m_OpMenu;
    QMenu *m_HelpMenu;

    QToolBar *m_FileToolBar;
    QToolBar *m_EditToolBar;

    QAction *m_NewAct;
    QAction *m_CollaborateAct;
    QAction *m_OpenAct;
    QAction *m_SaveAct;
    QAction *m_SaveAsAct;
    QAction *m_ExitAct;

    QAction *m_CutAct;
    QAction *m_CopyAct;
    QAction *m_PasteAct;

    QAction *m_AboutAct;

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
