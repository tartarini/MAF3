/*
 *  mafFindWidget.cpp
 *  mafGUI
 *
 *  Created by Roberto Mucci on 07/07/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafFindWidget.h"
#include <QMenu>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

using namespace mafGUI;

mafFindWidget::mafFindWidget(QWidget *parent) : QWidget(parent), m_FindLineEdit(NULL), m_ActionCase(NULL), m_ActionWhole(NULL) {

    m_ActionCase = new QAction(mafTr("Match case"), this);
    m_ActionCase->setCheckable(true);
    m_ActionWhole = new QAction(mafTr("Mach whole word"), this);
    m_ActionWhole->setCheckable(true);

    QToolBar *toolBar = new QToolBar(mafTr("Find Options"));
    toolBar->addAction(m_ActionCase);
    toolBar->addAction(m_ActionWhole);
    
    
    m_FindLineEdit = new QLineEdit;
    m_FindLineEdit->setFocus();
    m_FindLineEdit->setPlaceholderText("Find..");
    m_FindLineEdit->setMaximumWidth(150);

    QHBoxLayout *findLayout = new QHBoxLayout;
    findLayout->setMargin(0);
    findLayout->addStretch();
    findLayout->addWidget(toolBar,0, Qt::AlignRight);
    findLayout->addWidget(m_FindLineEdit);
    setLayout(findLayout);
}
