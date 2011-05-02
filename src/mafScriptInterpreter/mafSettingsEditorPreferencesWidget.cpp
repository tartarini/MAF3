/* mafSettingsEditorPreferencesWidget.cpp ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Dec 23 10:34:54 2009 (+0100)
 * Version: $Id$
 * Last-Updated: Wed Dec 23 10:50:29 2009 (+0100)
 *           By: Julien Wintz
 *     Update #: 11
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#include "mafSettingsEditor.h"
#include "mafSettingsEditorPreferencesWidget.h"

using namespace mafScriptInterpreter;

class mafScriptInterpreter::mafSettingsEditorPreferencesWidgetPrivate
{
public:
    mafSettingsEditor *editor;

    QDialogButtonBox *box;
};

mafSettingsEditorPreferencesWidget::mafSettingsEditorPreferencesWidget(QWidget *parent) : mafGUIApplicationSettingsPage(parent), d(new mafSettingsEditorPreferencesWidgetPrivate)
{
    d->editor = new mafSettingsEditor(this);
    d->editor->setSettings("inria", "maf");

    d->box = new QDialogButtonBox(this);
    d->box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(d->editor);
    layout->addWidget(d->box);
}

mafSettingsEditorPreferencesWidget::~mafSettingsEditorPreferencesWidget(void)
{
    delete d;
    d = NULL;
}
