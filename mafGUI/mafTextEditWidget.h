/*
 *  mafTextEditWidget.h
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 14/01/11.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTEXTEDITWIDGET_H
#define MAFTEXTEDITWIDGET_H

#include "mafGUIDefinitions.h"

namespace mafGUI {
/**
    Class Name: mafTextEditWidget
    This class extend functionalities of a text normal editor or browser:
    - add syntax highlighting
*/
class MAFGUISHARED_EXPORT mafTextEditWidget : public QWidget {
public:
    mafTextEditWidget(QWidget *parent = 0);

    mafTextEditWidget(QSyntaxHighlighter *highlighter, QWidget *parent = 0);

    void setHighlighter(QSyntaxHighlighter *highlighter);

    virtual ~mafTextEditWidget();

private:
    ///
    void initialize();
    QTextEdit *m_TextEditor;
    QSyntaxHighlighter *m_Highlighter;
};

} // namespace mafGUI

#endif // MAFTEXTEDITWIDGET_H
