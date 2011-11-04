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
#include <QLabel>

using namespace mafGUI;

mafWorkflowWidget::mafWorkflowWidget(QWidget *parent) : QWidget(parent), m_OpenWorkbenchButton(NULL), m_SubmitButton(NULL), m_WorkflowList(NULL), m_ParametersList(NULL) {

    QVBoxLayout *hbox = new QVBoxLayout(this);
    QVBoxLayout *vListbox = new QVBoxLayout();
    QHBoxLayout *hButtonbox = new QHBoxLayout(this);

    QLabel *workflowLabel = new QLabel(tr("Workflows:"));
    QLabel *parametersLabel = new QLabel(tr("Parameters:"));

    m_WorkflowList = new QListWidget(this);
    m_ParametersList = new QListWidget(this);
 
    m_OpenWorkbenchButton = new QPushButton("Open Taverna", this);
    m_SubmitButton = new QPushButton("Submit workflow", this);

    hButtonbox->setSpacing(1);
    hButtonbox->addStretch(1);
    hButtonbox->addWidget(m_OpenWorkbenchButton);
    hButtonbox->addWidget(m_SubmitButton);
    hButtonbox->addStretch(1);

    vListbox->addWidget(workflowLabel);
    vListbox->addWidget(m_WorkflowList);
    vListbox->addWidget(parametersLabel);
    vListbox->addWidget(m_ParametersList);

    hbox->addSpacing(5);
    hbox->addLayout(vListbox);
    hbox->addLayout(hButtonbox);

    setLayout(hbox);
}
