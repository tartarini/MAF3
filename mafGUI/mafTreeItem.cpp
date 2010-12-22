#include "mafTreeItem.h"

mafTreeItem::mafTreeItem(QObject *obj, bool done) : QStandardItem(obj->objectName())
{
    setCheckable(true);
    setCheckState(done ? Qt::Checked : Qt::Unchecked);
    setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|
             Qt::ItemIsEditable|Qt::ItemIsUserCheckable);
    m_Data = obj;
}
