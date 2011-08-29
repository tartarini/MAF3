/*
 *  mafInterpreterPreferencesWidget.h
 *  mafScriptEditor
 *
 *  Created by Daniele Giunchi and Paolo Quadrani on 08/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#ifndef MAFINTERPRETERPREFERENCESWIDGET_H
#define MAFINTERPRETERPREFERENCESWIDGET_H


#include "mafScriptInterpreter_global.h"
#include <mafGUIApplicationSettingsPage.h>

namespace mafScriptInterpreter {

class mafInterpreterConsole;
class mafInterpreterPreferencesWidgetPrivate;

/**
    Class Name: mafInterpreterPreferencesWidget
 It handles the preferences for the interpreter console:
 - text color
 - background color
 */
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafInterpreterPreferencesWidget : public mafGUI::mafGUIApplicationSettingsPage
{
    Q_OBJECT

public slots:
    /// set the font to the console
    void onFontChosen(QFont);
    /// called when the window size has been changed
    void onSizeChosen(QString);
    
    /// change color of the text
    void onTextColorClicked(void);
    /// change color of background
    void onBackgroundColorClicked(void);
    /// change background opacity
    void onBackgroundOpacityChanged(int value);

public:
    /// Object Constructor.
     mafInterpreterPreferencesWidget(mafInterpreterConsole *editor, QWidget *parent = 0);
    /// Destructor Constructor.
    ~mafInterpreterPreferencesWidget(void);

    /// key event handler.
    void keyPressEvent(QKeyEvent *event);

private:
    mafInterpreterPreferencesWidgetPrivate *m_PrivateClassPointer; ///< pimpl pattern.
};
    
} //end namespace

#endif // MAFINTERPRETERPREFERENCESWIDGET_H
