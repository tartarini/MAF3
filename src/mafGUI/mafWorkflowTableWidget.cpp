/*
 *  mafWorkflowTableWidget.cpp
 *  mafGUI
 *
 *  Created by Roberto Mucci on 07/07/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafWorkflowTableWidget.h"
#include <QMenu>
#include <QHBoxLayout>
#include <QHeaderView>


using namespace mafGUI;

mafWorkflowTableWidget::mafWorkflowTableWidget(QWidget *parent) : QWidget(parent), m_Table(NULL) {

    QHBoxLayout *hbox = new QHBoxLayout(this);
    QVBoxLayout *tablebox = new QVBoxLayout();

    m_Table = new QTableWidget();

    //Set table row count 1 and column count 3
    m_Table->setRowCount(5);
    m_Table->setColumnCount(7);

    //m_Table->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    //Set Header Label Texts Here
    m_Table->setHorizontalHeaderLabels(QString("WORKFLOW ID;STATUS;OWNER;CREATE TIME;START TIME;FINISH TIME;EXPIRY").split(";"));	

    //Add Table items here With Default Cell Texts
//     m_Table->setItem(0,0,new QTableWidgetItem("test"));
//     m_Table->setItem(0,1,new QTableWidgetItem("test"));
//     m_Table->setItem(0,2,new QTableWidgetItem("test"));

    m_Table->horizontalHeader()->setStretchLastSection(true);
    m_Table->resizeColumnsToContents();

    tablebox->addWidget(m_Table);
    hbox->addLayout(tablebox);
    setLayout(hbox);
}
