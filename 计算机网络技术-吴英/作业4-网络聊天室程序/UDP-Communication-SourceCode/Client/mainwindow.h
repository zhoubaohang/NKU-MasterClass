#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "udpclient.h"

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
    void do_login(QString, int, QString);
    void do_logout();
    void do_broadcast(QList<int>, QString);

private slots:
    void on_btn_login_toggled(bool checked);
    void on_update_users(QList<User>);

    void on_btn_push_clicked();

private:
    Ui::MainWindow *ui;
    UDPClient *m_client = nullptr;
};
#endif // MAINWINDOW_H
