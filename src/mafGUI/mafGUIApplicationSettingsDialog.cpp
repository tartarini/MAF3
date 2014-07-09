/*
 *  mafGUIApplicationSettingsDialog.cpp
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 21/01/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafGUIApplicationSettingsDialog.h"
#include "mafGUIApplicationSettingsPage.h"

#include <QtGui>
#include <QString>

using namespace mafGUI;

mafGUIApplicationSettingsDialog::mafGUIApplicationSettingsDialog() {
    m_ContentsWidget = new QListWidget;
    m_ContentsWidget->setViewMode(QListView::IconMode);
    m_ContentsWidget->setIconSize(QSize(96, 84));
    m_ContentsWidget->setMovement(QListView::Static);
    m_ContentsWidget->setMaximumWidth(118);
    m_ContentsWidget->setMinimumWidth(84);
	m_ContentsWidget->setMaximumHeight(300); 
	m_ContentsWidget->setMinimumHeight(250);
    m_ContentsWidget->setSpacing(12);
    m_ContentsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_PagesWidget = new QStackedWidget;
    m_PagesWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void mafGUIApplicationSettingsDialog::addPage(mafGUIApplicationSettingsPage *page) {
    QListWidgetItem *propButton = new QListWidgetItem(m_ContentsWidget);
    propButton->setIcon(page->pageIcon());
    propButton->setText(page->pageText());
    propButton->setTextAlignment(Qt::AlignHCenter);
    propButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    m_ContentsWidget->addItem(propButton);
    m_PagesWidget->addWidget(page);
}

void mafGUIApplicationSettingsDialog::insertPageAtIndex(mafGUIApplicationSettingsPage *page, int index) {
    QListWidgetItem *propButton = new QListWidgetItem();
    propButton->setIcon(page->pageIcon());
    propButton->setText(page->pageText());
    propButton->setTextAlignment(Qt::AlignHCenter);
    propButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    m_ContentsWidget->insertItem(index, propButton);
    m_PagesWidget->insertWidget(index, page);
}

void mafGUIApplicationSettingsDialog::removePageAtIndex(int index) {
    QListWidgetItem *item = m_ContentsWidget->item(index);
    if (item) {
        m_ContentsWidget->removeItemWidget(item);
    }
    QWidget *w = m_PagesWidget->widget(index);
    if(w != 0) {
        m_PagesWidget->removeWidget(w);
    }
}

void mafGUIApplicationSettingsDialog::setupSettingsDialog() {
    QPushButton *closeButton = new QPushButton(mafTr("Close"));
    m_ContentsWidget->setCurrentRow(0);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_ContentsWidget, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(changePage(QListWidgetItem *, QListWidgetItem *)));
                                     

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(m_ContentsWidget);
    horizontalLayout->addWidget(m_PagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(6);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(mafTr("Application settings"));
}

void mafGUIApplicationSettingsDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous) {
    if (!current)
        current = previous;

    m_PagesWidget->setCurrentIndex(m_ContentsWidget->row(current));
}
