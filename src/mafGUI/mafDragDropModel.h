/*
 *  mafDragDropModel.cpp
 *  mafGui
 *
 *  Created by Roberto Mucci on 11/03/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef mafDragDropModel_H
#define mafDragDropModel_H

// Includes list
#include "mafGUIDefinitions.h"
#include "mafTreeModel.h"

namespace mafGUI {
/**
Class Name: mafDragDropModel
This class supplied by a drag and drop operation inside tree.
*/

class MAFGUISHARED_EXPORT mafDragDropModel : public mafTreeModel
{
    Q_OBJECT

public:
    mafDragDropModel(/*const QStringList &strings,*/ QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent);
    QStringList mimeTypes() const;
    Qt::DropActions supportedDropActions() const;
};

} //end namespace

#endif
