#include "mafApplicationSettingsPageConfigurations.h"

#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>

using namespace mafGUI;

mafApplicationSettingsPageConfigurations::mafApplicationSettingsPageConfigurations(QWidget *parent) : mafGUIApplicationSettingsPage(parent) {
    m_PageText = mafTr("Configuration");
    m_PageIcon = QIcon(":/images/config.png");

    // Build the Page's GUI
   QGroupBox *configGroup = new QGroupBox(tr("Server configuration"));

    QLabel *serverLabel = new QLabel(tr("Server:"));
    QComboBox *serverCombo = new QComboBox;
    serverCombo->addItem(tr("Qt (Australia)"));
    serverCombo->addItem(tr("Qt (Germany)"));
    serverCombo->addItem(tr("Qt (Norway)"));
    serverCombo->addItem(tr("Qt (People's Republic of China)"));
    serverCombo->addItem(tr("Qt (USA)"));

    QHBoxLayout *serverLayout = new QHBoxLayout;
    serverLayout->addWidget(serverLabel);
    serverLayout->addWidget(serverCombo);

    QVBoxLayout *configLayout = new QVBoxLayout;
    configLayout->addLayout(serverLayout);
    configGroup->setLayout(configLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(configGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}
