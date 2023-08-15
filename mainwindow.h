#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <mqtt/qmqtt_client.h>
#include <QObject>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QNetworkAccessManager>

#include "sqldata.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Sqldata sql;

private slots:
    void on_ctl_pump_button_clicked();

    void on_ctl_fan_button_clicked();

    void on_ctl_auto_button_clicked();

    void on_test_button_clicked();



private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QMQTT::Client *client;
    QNetworkAccessManager _manager;

    const QString ipaddr="192.168.186.210";
    const quint16 port=1883;
    QHostAddress host;
    QString subscribetopic="qtmqtt";
    QString publishtopic="command";

    QStringList strlist;

    void change_prperties(QStringList strlist);
    void insertData(QStringList strlist);
    void saveFile(QString path,QList<Data> datalist);

    void sendSMS(const QString& phoneNumber, const QString& message);
};
#endif // MAINWINDOW_H
