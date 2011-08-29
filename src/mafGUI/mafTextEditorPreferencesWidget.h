/*
 *  mafTextEditorPreferencesWidget.h
 *  mafScriptEditor
 *
 *  Created by Daniele Giunchi and Paolo Quadrani on 08/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTEXTEDITORPREFERENCESWIDGET_H
#define MAFTEXTEDITORPREFERENCESWIDGET_H

#include "mafGUI_global.h"
#include "mafGUIApplicationSettingsPage.h"

namespace mafGUI {

class mafTextEditor;
class mafTextEditorPreferencesWidgetPrivate;

/**
 Class Name: mafTextEditorPreferencesWidget
 Preference widget for the common mafTextEditor.
 */
class MAFGUISHARED_EXPORT mafTextEditorPreferencesWidget : public mafGUIApplicationSettingsPage
{
    Q_OBJECT
    
public slots:
    /// set the font to the console
    void onFontChosen(QFont);
    /// called when the size is changed
    void onSizeChosen(QString);
    
    /// set the line numbers On or Off. 
    void onNumbersCkeckBoxChanged(int);
    /// set if the current line is highlighted.
    void onHighlightCkeckBoxChanged(int);
    // set if showing revisions.
    void onRevisionsCkeckBoxChanged(int);
    
    /// called when click OK button.
    void onOkButtonClicked(void);
    /// called when click Cancel button.
    void onCancelButtonClicked(void);

signals:
    /// signal emitted when the change is accepted.
    void accepted(void);
    /// signal emitted when the change is rejected.
    void rejected(void);

public:
    /// Object Constructor.
     mafTextEditorPreferencesWidget(mafTextEditor *editor, QWidget *parent);
    /// Object Destructor.
    ~mafTextEditorPreferencesWidget(void);

    /// key event handler.
    void keyPressEvent(QKeyEvent *event);

private:
    mafTextEditorPreferencesWidgetPrivate *m_PrivateClassPointer; ///< pimpl pattern.
};
    
} // end namespace

#endif // MAFTEXTEDITORPREFERENCESWIDGET_H
