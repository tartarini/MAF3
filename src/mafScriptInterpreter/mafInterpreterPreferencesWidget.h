/* mafInterpreterPreferencesWidget.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Apr 10 09:24:10 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Thu Aug  6 10:21:27 2009 (+0200)
 *           By: Julien Wintz
 *     Update #: 11
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef MAFINTERPRETERPREFERENCESWIDGET_H
#define MAFINTERPRETERPREFERENCESWIDGET_H


#include "mafScriptInterpreter_global.h"
#include <mafGUIApplicationSettingsPage.h>

namespace mafScriptInterpreter {

class mafInterpreterConsole;
class mafInterpreterPreferencesWidgetPrivate;

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafInterpreterPreferencesWidget : public mafGUI::mafGUIApplicationSettingsPage
{
    Q_OBJECT

public:
     mafInterpreterPreferencesWidget(mafInterpreterConsole *editor, QWidget *parent = 0);
    ~mafInterpreterPreferencesWidget(void);

    void keyPressEvent(QKeyEvent *event);

public slots:
    void onFontChosen(QFont);
    void onSizeChosen(QString);

    void onTextColorClicked(void);
    void onBackgroundColorClicked(void);
    void onBackgroundOpacityChanged(int value);

private:
    mafInterpreterPreferencesWidgetPrivate *m_PrivateClassPointer;
};
    
} //end namespace

#endif // MAFINTERPRETERPREFERENCESWIDGET_H
