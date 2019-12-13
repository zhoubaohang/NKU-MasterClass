#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <udpserver.h>
#include <QMessageBox>
#include <QUdpSocket>

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
    void control_connect(bool, QString, quint16);

private slots:
    void on_pushButton_toggled(bool checked);
    void on_add_user(User);
    void on_delete_user(int);

private:
    Ui::MainWindow *ui;

    UDPServer *m_server = nullptr;
};
#endif // MAINWINDOW_H
