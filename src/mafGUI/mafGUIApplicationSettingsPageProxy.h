/*
 *  mafGUIApplicationSettingsPageProxy.h
 *  mafGUI
 *
 *  Created by Robero Mucci on 02/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFGUIAPPLICATIONSETTINGSPAGEPROXY_H
#define MAFGUIAPPLICATIONSETTINGSPAGEPROXY_H

#include <mafGUIApplicationSettingsPage.h>
#include <QLineEdit>
#include <QCheckBox>

namespace mafGUI {

/**
 Class name: mafGUIApplicationSettingsPageProxy
 This class represent the settings page for configuration of proxy.
*/
class MAFGUISHARED_EXPORT mafGUIApplicationSettingsPageProxy : public mafGUI::mafGUIApplicationSettingsPage {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafGUI::mafGUIApplicationSettingsPage);
       
public:
    /// Object constructor.
    mafGUIApplicationSettingsPageProxy(QWidget *parent = 0);

private  Q_SLOTS:
    /// A slot connected with checkbox signal.
    void slotCheckBox(int state);

    /// Allows to write the updated preferences.
    void writeSettings(void);

private:
    /// Allows to read the saved preferences.
    void readSettings(void);

    QLineEdit *m_ProxyHostLineEdit; ///< Proxy host.
    QLineEdit *m_ProxyPortLineEdit; ///< Proxy port.
    QCheckBox *m_Checkbox; ///< Checkbox to enable proxy
};

} // namespace mafGUI

#endif // MAFGUIAPPLICATIONSETTINGSPAGEPROXY_H
