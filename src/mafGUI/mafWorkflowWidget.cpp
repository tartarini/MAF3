/*
 *  mafWorkflowWidget.cpp
 *  mafGUI
 *
 *  Created by Roberto Mucci on 07/07/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafWorkflowWidget.h"
#include <QMenu>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

using namespace mafGUI;

mafWorkflowWidget::mafWorkflowWidget(QWidget *parent) : QWidget(parent), m_OpenWorkbenchButton(NULL), m_SubmitButton(NULL), m_RemoveButton(NULL), m_WorkflowList(NULL) /*m_ActionCase(NULL), m_ActionWhole(NULL)*/ {

    QVBoxLayout *vbox = new QVBoxLayout();
    QHBoxLayout *hbox = new QHBoxLayout(this);

    m_WorkflowList = new QListWidget(this);
 
    m_OpenWorkbenchButton = new QPushButton("Open Taverna Workbench", this);
    m_SubmitButton = new QPushButton("Submit workflow", this);
    m_RemoveButton = new QPushButton("Remove workflow", this);


    vbox->setSpacing(3);
    vbox->addStretch(1);
    vbox->addWidget(m_OpenWorkbenchButton);
    vbox->addWidget(m_SubmitButton);
    vbox->addWidget(m_RemoveButton);
    vbox->addStretch(1);

    hbox->addWidget(m_WorkflowList);
    hbox->addSpacing(15);
    hbox->addLayout(vbox);

    setLayout(hbox);
}
