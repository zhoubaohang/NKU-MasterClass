#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMap>
#include <QFile>
#include <QDebug>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QMainWindow>
#include <QTableWidget>
#include "pcap.h"
#include "netadaption.h"
#include "network.h"

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
    void comboBox_loaded(QComboBox*);
    void craw_package(QTableWidget*, QStatusBar*, QMap<QString, QString>, bool);

private slots:
    void on_actionAbout_triggered();
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_pushButton_toggled(bool checked);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    NetAdaption *p_netadaption;
    Network *p_network;

};
#endif // MAINWINDOW_H
