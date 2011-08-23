/* mafTextEditorPreferencesWidget.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Apr 10 09:24:10 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Thu Aug  6 10:20:51 2009 (+0200)
 *           By: Julien Wintz
 *     Update #: 8
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef MAFTEXTEDITORPREFERENCESWIDGET_H
#define MAFTEXTEDITORPREFERENCESWIDGET_H

#include "mafGUI_global.h"
#include "mafGUIApplicationSettingsPage.h"

namespace mafGUI {

class mafTextEditor;
class mafTextEditorPreferencesWidgetPrivate;

class MAFGUISHARED_EXPORT mafTextEditorPreferencesWidget : public mafGUIApplicationSettingsPage
{
    Q_OBJECT

public:
     mafTextEditorPreferencesWidget(mafTextEditor *editor, QWidget *parent);
    ~mafTextEditorPreferencesWidget(void);

    void keyPressEvent(QKeyEvent *event);

signals:
    void accepted(void);
    void rejected(void);

public slots:
    void onFontChosen(QFont);
    void onSizeChosen(QString);

    void onNumbersCkeckBoxChanged(int);
    void onHighlightCkeckBoxChanged(int);
    void onRevisionsCkeckBoxChanged(int);

    void onOkButtonClicked(void);
    void onCancelButtonClicked(void);

private:
    mafTextEditorPreferencesWidgetPrivate *d;
};
    
} // end namespace

#endif // MAFTEXTEDITORPREFERENCESWIDGET_H
