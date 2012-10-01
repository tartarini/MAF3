/*
 *  mafFindWidget.h
 *  mafGUI
 *
 *  Created by Roberto Mucci on 07/07/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFFINDWIDGET_H
#define MAFFINDWIDGET_H

// Includes list
#include "mafGUIDefinitions.h"
#include <QAction>
#include <QLineEdit>

namespace mafGUI {
/**
Class Name: mafFindWidget
It represents a widget to search words inside a text.
*/
class MAFGUISHARED_EXPORT mafFindWidget : public QWidget {
    Q_OBJECT
public:
    /// Object Constructor
    mafFindWidget(QWidget *parent = 0);
    QLineEdit *m_FindLineEdit; ///< Area in which insert word to search
    QAction *m_ActionCase; ///< Match case menu.
    QAction *m_ActionWhole; ///< Match whole word menu.

};

} //end namespace

#endif // MAFFINDWIDGET_H
