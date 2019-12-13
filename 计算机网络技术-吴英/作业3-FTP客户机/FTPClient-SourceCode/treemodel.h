#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QIcon>
#include <QDebug>
#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>

class TreeModel:public QObject
{
    Q_OBJECT

public:
    TreeModel(QTreeView *);
    void clear();
    QStandardItem *itemFromIndex(QModelIndex);

public slots:
    void on_list_checked(QVector<QStringList>);
    void on_list_checked(const QModelIndex&, QVector<QStringList>);
    void on_delete_treeview_item(const QModelIndex&);
//    void on_add_treeview_item(const QModelIndex&, QString);

private:
    QStandardItemModel *p_model = nullptr;
    QTreeView *p_tree = nullptr;
};

#endif // TREEMODEL_H
