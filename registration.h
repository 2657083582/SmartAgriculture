#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QMainWindow>
#include "logindata.h"
#include "mainwindow.h"

namespace Ui {
class registration;
}

class registration : public QMainWindow
{
    Q_OBJECT

public:
    explicit registration(QWidget *parent = nullptr);
    loginData *logindata;
    ~registration();

private slots:
//    void on_pushButton_clicked();

//    void on_pushButton_2_clicked();

//    void on_label_2_clicked();

    void on_label_cancel_clicked();

    void on_label_submit_clicked();

    void on_label_cancel_hovered();

    void on_label_cancel_unhovered();

    void on_label_submit_hovered();

    void on_label_submit_unhovered();

private:
    Ui::registration *ui;
    MainWindow w;
//    login *log;
};

#endif // REGISTRATION_H
