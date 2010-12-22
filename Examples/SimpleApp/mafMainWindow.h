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

namespace Ui {
    class mafMainWindow;
}

class GoogleChat;

class mafMainWindow : public QMainWindow {
    Q_OBJECT
public:
    mafMainWindow(QWidget *parent = 0);
    ~mafMainWindow();

public slots:
    /// Create the MDI sub window to host the mafView.
    void createViewWindow();

    /// Open the Google Talk interface.
    void openGoogleTalk();

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

protected:
    /// Method used to catch events related to changes.
    void changeEvent(QEvent *e);

    /// Method used to catch the close application's event
    void closeEvent(QCloseEvent *event);

private:
    Ui::mafMainWindow *ui;

    mafGUI::mafGUIManager *m_GUIManager; ///< Manager for the GUI widgets creation and initialization.

    GoogleChat *googleChat;

    /// Connect the slots to the signal defined into the mafGUIManager through the mafEventBus
    void connectCallbacks();

    /// Check if the application needs to save data.
    int maybeSave();
};

#endif // MAFMAINWINDOW_H
