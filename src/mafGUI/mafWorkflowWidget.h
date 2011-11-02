/*
 *  mafWorkflowWidget.h
 *  mafGUI
 *
 *  Created by Roberto Mucci on 07/07/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFWORKFLOWWIDGET_H
#define MAFWORKFLOWWIDGET_H

// Includes list
#include "mafGUIDefinitions.h"
#include <QAction>
#include <QListWidget>
#include <QPushButton>

namespace mafGUI {
/**
Class Name: mafWorkflowWidget
It represents a widget to manage an external workflow workbench.
*/
class MAFGUISHARED_EXPORT mafWorkflowWidget : public QWidget {
    Q_OBJECT
public:
    /// Object Constructor
    mafWorkflowWidget(QWidget *parent = 0);
    QPushButton *m_OpenWorkbenchButton; ///< Open external workbench (Taverna).
    QPushButton *m_SubmitButton; ///< Submit workflow.
    QListWidget *m_WorkflowList; ///< List of available workflow.
    QListWidget *m_ParametersList; ///< List of parameters file.
};

} //end namespace

#endif // MAFWORKFLOWWIDGET_H
