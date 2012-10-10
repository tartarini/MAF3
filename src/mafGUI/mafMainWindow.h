/*
 *  mafMainWindow.h
 *  SimpleApp
 *
 *  Created by Paolo Quadrani on 14/12/10.
 *  Copyright 2010 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMAINWINDOW_H
#define MAFMAINWINDOW_H

#include <QtGui>
#include <QMainWindow>

#include "mafGUIManager.h"
#include "mafDragDropModel.h"
#include "mafGUIRegistration.h"
#include "mafMdiSubWindow.h"
#include "mafTreeWidget.h"
#include "mafFindWidget.h"
#include "mafTextEditWidget.h"

#include <mafLogic.h>

namespace Ui {
    class mafMainWindow;
}


/**
  Class Name: mafMainWindow
  This class represent the Main Window of the vertical application.
  */
class MAFGUISHARED_EXPORT mafMainWindow : public QMainWindow {
    Q_OBJECT
public:
    /// Object constructor
    mafMainWindow(QWidget *parent = 0);

    /// Object constructor
    mafMainWindow(mafApplicationLogic::mafLogic *logic, QWidget *parent = 0);

    /// Object destructor
    ~mafMainWindow();

    /// Allow to assign the application's logic.
    void setLogic(mafApplicationLogic::mafLogic *logic);

    /// Return the mafGUIManager
    mafGUI::mafGUIManager *guiManager();

    /// Allows to add a settings page to the settings dialog.
    void plugApplicationSettingsPage(mafGUI::mafGUIApplicationSettingsPage *page);

    /// Performs all the necessary steps to setup settings and synchronize all GUI elements to get ready to start, then show the main window.
    void setupMainWindow();

public Q_SLOTS:

    /// Called when the view is going to be selected by the mouse
    void viewWillBeSelected();

    /// Called when the mouse start interacting with the view.
    void subWindowSelected(QMdiSubWindow *sub_win);

    /// Set name of the application.
    void updateApplicationName();

protected Q_SLOTS:
    /// Observe the viewCreatedSignal emitted by the mafViewManager.
    /* Attach the render context to the GUI widget */
    virtual void viewCreated(mafCore::mafObjectBase *view);

    /// Slot called when dynamic loaded GUI is available.
    virtual void loadedGUIAvailable(int type, QWidget *w);

    /// Slot called when a dynamic loaded GUI needs to be removed from the panel.
    virtual void loadedGUIToRemove(int type);

private Q_SLOTS:
    /// Allow to call the code to save the user data through the logic module.
    bool save();

    /// Load application's settings
    void readSettings();

    /// Write the application's settings
    void writeSettings();

    /// Shows the About dialog.
    void showAbout();
    
    /// Called when a subWindow has been destroyed.
    void subWindowDestroyed();

protected:
    /// Method used to catch events related to changes.
    void changeEvent(QEvent *e);

    /// Method used to catch the close application's event
    void closeEvent(QCloseEvent *event);

private:
    Ui::mafMainWindow *ui;

    mafGUI::mafGUIManager *m_GUIManager; ///< Manager for the GUI widgets creation and initialization.
    mafGUI::mafTreeWidget  *m_Tree; ///< Manage the resource's tree hierarchy.
    mafGUI::mafFindWidget *m_FindWidget; ///< Widget able to find words in a text.
    mafGUI::mafDragDropModel  *m_Model; ///< Tree model of the mafResources with drag&Drop operation.

    mafGUI::mafTextEditWidget *m_LogWidget;

    mafApplicationLogic::mafLogic *m_Logic; ///< Application's logic.

    QHash<QMdiSubWindow *, mafCore::mafObjectBase *> m_ViewSubWindowHash; ///< hash which binds mafViews with qt sub windows
    
    /// Connect the slots to the signal defined into the mafGUIManager through the mafEventBus
    void connectCallbacks();

    /// Initialize the Main Window.
    void initializeMainWindow();

    /// Check if the application needs to save data.
    int maybeSave();
};

#endif // MAFMAINWINDOW_H
