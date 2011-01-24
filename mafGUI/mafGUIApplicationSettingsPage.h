/*
 *  mafGUIApplicationSettingsPage.h
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 21/01/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFGUIAPPLICATIONSETTINGSPAGE_H
#define MAFGUIAPPLICATIONSETTINGSPAGE_H

// Includes list
#include "mafGUIDefinitions.h"
#include <QWidget>
#include <QIcon>

namespace mafGUI {

/**
 Class name: mafGUIApplicationSettingsPage
 This class represent the base point for Application's settings page. The setting
 Dialog will insert the icon returned by the page into the setting's list and will
 add also a text associated to the variable m_PageText.
*/
class MAFGUISHARED_EXPORT mafGUIApplicationSettingsPage : public QWidget {
    Q_OBJECT

public:
    /// Object constructor.
    mafGUIApplicationSettingsPage(QWidget *parent = 0);

    /// Return the icon associated with the setting's page.
    const QIcon &pageIcon() const;

    /// Return the text title associated with the text (the object's name)
    const mafString &pageText() const;

protected:
    mafString m_PageText; ///< text that will be shown into the dialog setting's list
    QIcon m_PageIcon; ///< Icon image associated with the setting page.
};

inline const mafString &mafGUIApplicationSettingsPage::pageText() const {
    return m_PageText;
}

inline const QIcon &mafGUIApplicationSettingsPage::pageIcon() const {
    return m_PageIcon;
}

} // namespace mafGUI

#endif // MAFGUIAPPLICATIONSETTINGSPAGE_H
