/*
 *  mafGUIApplicationSettingsDialog.h
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 21/01/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFGUIAPPLICATIONSETTINGSDIALOG_H
#define MAFGUIAPPLICATIONSETTINGSDIALOG_H

// Includes list
#include "mafGUIDefinitions.h"
#include <QDialog>

class QListWidget;
class QListWidgetItem;
class QStackedWidget;

namespace mafGUI {

class mafGUIApplicationSettingsPage;

/**
 Class name: mafGUIApplicationSettingsDialog
 This class represent the manager for Application's settings dialog.
*/
class MAFGUISHARED_EXPORT mafGUIApplicationSettingsDialog : public QDialog {
    Q_OBJECT

public:
    /// Object constructor.
    mafGUIApplicationSettingsDialog();

    /// Add a settings page to the dialog
    void addPage(mafGUIApplicationSettingsPage *page);

    /// Insert a settings' page at the given index.
    void insertPageAtIndex(mafGUIApplicationSettingsPage *page, int index);

    /// Remove the application's setting placed at given index.
    void removePageAtIndex(int index);

    /// setup the application's settings dialog.
    /** This method has to be called after that all whe pages has been added
    to the dialog. The method will provide the loading of page's icons and layout
    of the widgets present in it.*/
    void setupSettingsDialog();

public slots:
    /// Allow to update the shown page according to the item selected on icon's list.
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
    void createIcons();

    QListWidget *m_ContentsWidget; ///< List of icons corresponding to the settings pages
    QStackedWidget *m_PagesWidget; ///< Widget containing the stack of application's settings pages.
};

} // namespace mafGUI

#endif // MAFGUIAPPLICATIONSETTINGSDIALOG_H
