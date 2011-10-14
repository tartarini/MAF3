/*
 *  mafWorkflowTableWidget.h
 *  mafGUI
 *
 *  Created by Roberto Mucci on 07/07/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFWORKFLOWTABLEWIDGET_H
#define MAFWORKFLOWTABLEWIDGET_H

// Includes list
#include "mafGUIDefinitions.h"
#include <QAction>
#include <QTableWidget>

namespace mafGUI {
/**
Class Name: mafWorkflowTableWidget
It represents a widget to search words inside a text.
*/
class MAFGUISHARED_EXPORT mafWorkflowTableWidget : public QWidget {
    Q_OBJECT
public:
    /// Object Constructor
    mafWorkflowTableWidget(QWidget *parent = 0);
    QTableWidget* m_Table; ///< Table widget.


};

} //end namespace

#endif // MAFWORKFLOWTABLEWIDGET_H
