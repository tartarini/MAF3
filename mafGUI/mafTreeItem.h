#ifndef MAFTREEITEM_H
#define MAFTREEITEM_H

/*
    Copyright (c) 2009-10 Qtrac Ltd. All rights reserved.

    This program or module is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version. It is provided
    for educational purposes and is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
    the GNU General Public License for more details.
*/

#include <QStandardItem>

class mafTreeItem : public QStandardItem
{
public:
    mafTreeItem(QObject *obj, bool done = false);

    QObject *data() const { return m_Data; }

private:
    QObject * m_Data;
};


#endif // MAFTREEITEM_H
