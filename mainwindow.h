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
#include <QMouseEvent>

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

//    void on_ctl_auto_button_clicked();

    void on_quit_label_clicked();

    void on_min_label_clicked();

    void on_quit_label_hovered();

    void on_quit_label_unhovered();

    void on_min_label_hovered();

    void on_min_label_unhovered();

    void on_export_data_label_clicked();

    void on_auto_label_clicked();

    void on_auto_label_hovered();

    void on_auto_label_unhovered();

    void on_export_data_label_hovered();

    void on_export_data_label_unhovered();

    void on_temp_edit_clicked();

    void on_lineEdit_temp_submit();

    void on_lineEdit_temp_cancel();

    void on_lineEdit_humi_submit();

    void on_humi_edit_clicked();

    void on_lineEdit_humi_cancel();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QMQTT::Client *client;
    QNetworkAccessManager _manager;
    QPointF m_lastPos;

    const QString ipaddr="192.168.186.210";
    const quint16 port=1883;
    QHostAddress host;
    QString subscribetopic="qtmqtt";
    QString publishtopic="command";

    QStringList strlist;

    void change_prperties(QStringList strlist);
    void insertData(QStringList strlist);
    void saveFile(QString path,QList<Data> datalist);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

};
#endif // MAINWINDOW_H
