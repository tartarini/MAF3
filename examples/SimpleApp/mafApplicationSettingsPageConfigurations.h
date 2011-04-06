#ifndef MAFAPPLICATIONSETTINGSPAGECONFIGURATIONS_H
#define MAFAPPLICATIONSETTINGSPAGECONFIGURATIONS_H

#include <mafGUIApplicationSettingsPage.h>

/**
 Class name: mafApplicationSettingsPageConfigurations
 This class represent the settings page for general configuration of the mafMainWindow.
*/
class mafApplicationSettingsPageConfigurations : public mafGUI::mafGUIApplicationSettingsPage {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafGUI::mafGUIApplicationSettingsPage);

public:
    /// Object constructor.
    mafApplicationSettingsPageConfigurations(QWidget *parent = 0);
};

#endif // MAFAPPLICATIONSETTINGSPAGECONFIGURATIONS_H
