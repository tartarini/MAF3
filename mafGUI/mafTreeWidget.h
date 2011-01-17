/*
 *  mafTreeWidget.h
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTREEWIDGET_H
#define MAFTREEWIDGET_H

// Includes list
#include "mafGUIDefinitions.h"
#include <QTreeWidget>
#include <QStandardItemModel>

namespace mafGUI {
/**
Class Name: mafTreeWidget
It represents the view in model-view pattern, and it visualize che model
stored in mafTreeModel.
*/
class MAFGUISHARED_EXPORT mafTreeWidget : public QTreeView {
    Q_OBJECT
public:
    /// Object Constructor
    mafTreeWidget(QWidget *parent = 0);

public slots:
    //void refresh(void);

private slots:
    //void updateItem(...);

private:

};

} //end namespace

#endif // MAFTREEWIDGET_H
