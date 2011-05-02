/* mafSettingsEditorPreferencesWidget.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Dec 23 10:31:38 2009 (+0100)
 * Version: $Id$
 * Last-Updated: Wed Dec 23 10:36:27 2009 (+0100)
 *           By: Julien Wintz
 *     Update #: 8
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef MAFSETTINGSEDITORPREFERENCESWIDGET_H
#define MAFSETTINGSEDITORPREFERENCESWIDGET_H

#include "mafScriptInterpreter_global.h"
#include <mafGUIApplicationSettingsPage.h>

#include <QtCore>
#include <QtGui>

namespace mafScriptInterpreter {

class mafSettingsEditorPreferencesWidgetPrivate;

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafSettingsEditorPreferencesWidget : public mafGUI::mafGUIApplicationSettingsPage
{
    Q_OBJECT

public:
     mafSettingsEditorPreferencesWidget(QWidget *parent);
    ~mafSettingsEditorPreferencesWidget(void);

private:
    mafSettingsEditorPreferencesWidgetPrivate *d;
};

} //end namespace

#endif
