#ifndef MAFTREEMODEL_H
#define MAFTREEMODEL_H

#include "mafTreeItem.h"
#include <mafHierarchy.h>

class mafTreeModel : public QStandardItemModel
{
    Q_OBJECT

public:
    enum Insert {AtTopLevel, AsSibling, AsChild};

    mafTreeModel(QObject *parent=0);
    mafTreeModel(int row, int col);

    mafCore::mafHierarchy *hierarchy() const { return m_Hierarchy; }
    void setHierarchy(mafCore::mafHierarchy *hierarchy);
    void clear();

    mafTreeItem *insertNewItem(Insert insert,
                               QObject *obj, const QModelIndex &index);

signals:
    void load();
    void save();

private:
    void initialize();
    void buildModel(bool init = true);

    mafTreeItem *createNewItem(mafTreeItem *root,
                               QObject *obj, bool checked);

    mafCore::mafHierarchy *m_Hierarchy;
    mafTreeItem *m_CurrentItem;
};


#endif // MAFTREEMODEL_H
