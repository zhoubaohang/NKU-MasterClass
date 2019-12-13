#include "treemodel.h"

TreeModel::TreeModel(QTreeView *tree)
{
    this->p_tree = tree;
    this->p_model = new QStandardItemModel();
}

void TreeModel::on_list_checked(QVector<QStringList> files)
{
    this->p_model->clear();
    for (QStringList file: files)
    {
        QString type = file[2] == "<DIR>"?"DIR":"FILE";

        QStandardItem *item = new QStandardItem(QString("%1\t%2 %3\t%4")
                                                .arg(file[3]).arg(type).arg(file[0]).arg(file[1]));
        item->setIcon(QIcon(QString(":\\picture\\%1.jpg").arg(type.toLower())));
        this->p_model->appendRow(item);
    }
    this->p_model->setHeaderData(0, Qt::Horizontal, "名称\t类型\t时间");
    this->p_tree->setModel(this->p_model);
}

void TreeModel::on_list_checked(const QModelIndex &index, QVector<QStringList> files)
{
    QStandardItem *parent = this->p_model->itemFromIndex(index);
    parent->removeRows(0, parent->rowCount());

    for (QStringList file: files)
    {
        QString type = file[2] == "<DIR>"?"DIR":"FILE";
        QStandardItem *item = new QStandardItem(QString("%1\t%2 %3\t%4")
                                                .arg(file[3]).arg(type).arg(file[0]).arg(file[1]));
        item->setIcon(QIcon(QString(":\\picture\\%1.jpg").arg(type.toLower())));
        parent->appendRow(item);
    }
}

void TreeModel::clear()
{
    this->p_model->clear();
}

QStandardItem* TreeModel::itemFromIndex(QModelIndex index)
{
    return this->p_model->itemFromIndex(index);
}

void TreeModel::on_delete_treeview_item(const QModelIndex &index)
{
    this->p_model->removeRow(index.row(), index.parent());
}
