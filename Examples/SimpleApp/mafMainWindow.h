/*
 *  mafMainWindow.h
 *  SimpleApp
 *
 *  Created by Paolo Quadrani on 14/12/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMAINWINDOW_H
#define MAFMAINWINDOW_H

#include <QtGui>
#include <QVTKWidget.h>
#include <QMainWindow>

#include <mafGUIManager.h>
#include <mafDragDropModel.h>
#include <mafMdiSubWindow.h>
#include <mafTreeWidget.h>
#include <mafTextEditWidget.h>
#include <mafLogic.h>

namespace Ui {
    class mafMainWindow;
}

class GoogleChat;

/**
  Class Name: mafMainWindow
  This class represent the Main Window of the vertical application.
  */
class mafMainWindow : public QMainWindow {
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
    mafGUI::mafGUIManager *guiManager() const;

    /// Allows to add a settings page to the settings dialog.
    void plugApplicationSettingsPage(mafGUI::mafGUIApplicationSettingsPage *page);

    /// Performs all the necessary steps to setup settings and synchronize all GUI elements to get ready to start, then show the main window.
    void setupMainWindow();

public slots:
    /// Show/Hide the collaboration dock widget.
    void updateCollaborationDockVisibility(bool visible);

    /// Called when the view is going to be selected by the mouse
    void viewWillBeSelected();

    /// Called when the mouse start interacting with the view.
    void subWindowSelected(QMdiSubWindow *sub_win);

protected slots:
    /// Observe the viewCreatedSignal emitted by the mafViewManager.
    virtual void viewCreated(mafCore::mafObjectBase *view);

    /// Observe the viewSlectedSignal emitted by the mafViewManager.
    void viewSelected(mafCore::mafObjectBase *view);

    /// Slot called when dynamic loaded GUI is available.
    virtual void loadedGUIAvailable(int type, QWidget *w);

    /// Slot called when a dynamic loaded GUI needs to be removed from the panel.
    virtual void loadedGUIToRemove(int type);

private slots:
    /// Allow to call the code to save the user data through the logic module.
    bool save();

    /// Load application's settings
    void readSettings();

    /// Write the application's settings
    void writeSettings();

    /// Shows the About dialog.
    void showAbout();

protected:
    /// Method used to catch events related to changes.
    void changeEvent(QEvent *e);

    /// Method used to catch the close application's event
    void closeEvent(QCloseEvent *event);

private:
    Ui::mafMainWindow *ui;

    mafGUI::mafGUIManager *m_GUIManager; ///< Manager for the GUI widgets creation and initialization.
    mafGUI::mafTreeWidget  *m_Tree; ///< Manage the resource's tree hierarchy.
    mafGUI::mafDragDropModel  *m_Model; ///< Tree model of the mafResources with drag&Drop operation.

    mafGUI::mafTextEditWidget *m_LogWidget;

    mafApplicationLogic::mafLogic *m_Logic; ///< Application's logic.

    QDockWidget *m_DockGoogleChat; ///< Dock widget containing the google chat widget.

    /// Connect the slots to the signal defined into the mafGUIManager through the mafEventBus
    void connectCallbacks();

    /// Initialize the Main Window.
    void initializeMainWindow();

    /// Check if the application needs to save data.
    int maybeSave();
};

inline mafGUI::mafGUIManager *mafMainWindow::guiManager() const {
    return m_GUIManager;
}

#endif // MAFMAINWINDOW_H
