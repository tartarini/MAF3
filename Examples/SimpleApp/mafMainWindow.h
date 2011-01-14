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
#include <mafMdiSubWindow.h>
#include <mafTreeWidget.h>
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

public slots:
    /// Create the MDI sub window to host the mafView.
    void createViewWindow();

    /// Show/Hide the collaboration dock widget.
    void updateCollaborationDockVisibility(bool visible);

    /// Called when the view is going to be selected by the mouse
    void viewWillBeSelected();

    /// Called when the mouse start interacting with the view.
    void viewSelected(QMdiSubWindow *sub_win);

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
    mafGUI::mafTreeModel  *m_Model; ///< Tree model of the mafResources.

    mafApplicationLogic::mafLogic *m_Logic; ///< Application's logic.

    QDockWidget *m_DockGoogleChat; ///< Dock widget containing the google chat widget.

    /// Connect the slots to the signal defined into the mafGUIManager through the mafEventBus
    void connectCallbacks();

    /// Initialize the Main Window.
    void initializeMainWindow();

    /// Check if the application needs to save data.
    int maybeSave();
};

#endif // MAFMAINWINDOW_H
