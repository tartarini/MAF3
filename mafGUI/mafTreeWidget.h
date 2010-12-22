#ifndef MAFTREEWIDGET_H
#define MAFTREEWIDGET_H

#include <QTreeWidget>
#include <QStandardItemModel>

class mafTreeWidget : public QTreeView {
    Q_OBJECT
public:
    mafTreeWidget(QWidget *parent = 0);
    //void setHierarchy(QStandardItemModel * hierarchy);

public slots:
    //void refresh(void);

private slots:
    //void updateItem(QTreeWidgetItem *item);

private:
    /*void updateChildItems(QTreeWidgetItem *parent);
    QTreeWidgetItem *createItem(const QString &text, QTreeWidgetItem *parent, int index);
    QTreeWidgetItem *childAt(QTreeWidgetItem *parent, int index);
    int numberOfChildren(QTreeWidgetItem *parent);
    int findChild(QTreeWidgetItem *parent, const QString &text, int startIndex);

    QStandardItemModel *m_hierarchy;*/
};

#endif // MAFTREEWIDGET_H
