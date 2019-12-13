#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QTreeView>
#include <QMainWindow>
#include "cftp.h"
#include "treemodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void init_connections(QString, QString, QString);
    void delete_action(const QModelIndex&);
    void upload_action(const QModelIndex&, QByteArray, QString);

private slots:
    void on_pushButton_toggled(bool checked);
    void on_file_saved(QByteArray, QString);
    void on_upload_action();
    void on_delete_action();
    void on_treeView_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    CFtp *cftp = nullptr;
    QModelIndex curr_index;
    TreeModel *p_filetree = nullptr;

};
#endif // MAINWINDOW_H
