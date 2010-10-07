#ifndef MAFMAINWINDOW_H
#define MAFMAINWINDOW_H

#include <QtGui>
#include <QVTKWidget.h>
#include <QMainWindow>
#include <mafCoreDefinitions.h>
#include "mafMementoApplication.h"

namespace Ui {
    class mafMainWindow;
}

class mafMainWindow : public QMainWindow {
    Q_OBJECT
public:
    mafMainWindow(QWidget *parent = 0);
    ~mafMainWindow();

    /// Allow to assign the settings' filename.
    void setSettingsFilename(mafString filename);

public slots:
    /// Create the MDI sub window to host the mafView.
    void createViewWindow();

    /// Return the instance of the mafMainWindow's status. The caller has to delete the allocated memento memory he asked for.
    mafCore::mafMemento *createMemento() const;

    /// Called when the view is going to be selected by the mouse
    void viewWillBeSelected();

    /// Called when the mouse start interacting with the view.
    void viewSelected(QMdiSubWindow *sub_win);

protected:
    /// Method used to catch events related to changes.
    void changeEvent(QEvent *e);

    /// Method used to catch the close application's event
    void closeEvent(QCloseEvent *event);

    void mousePressEvent( QMouseEvent * event );

    void moveEvent ( QMoveEvent * event );

    void mouseReleaseEvent ( QMouseEvent * event );

    /// Create the actions associated with the menu items.
    virtual void createActions();

    /// Create the menu for the vertical application.
    virtual void createMenus();

    /// Create the toolbar for the vertical application.
    virtual void createToolBars();

private slots:
    /// Allow to call the code to save the user data through the logic module.
    bool save();

private:
    Ui::mafMainWindow *ui;
    mafString m_SettingsFilename; ///< Name of the settings file.

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *opMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;

    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;

    QAction *aboutAct;

    /// Load application's settings
    void readSettings();

    /// Write the application's settings
    void writeSettings();

    /// Check if the application needs to save data.
    bool maybeSave();
};

inline void mafMainWindow::setSettingsFilename(mafString filename) {
    m_SettingsFilename = filename;
    readSettings();
}

#endif // MAFMAINWINDOW_H
