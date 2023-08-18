#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "logindata.h"
#include "registration.h"
#include "mainwindow.h"

namespace Ui {
    class login;
}

class login : public QMainWindow
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    loginData *logindata;
    void setUser(QString userName, QString password);
    ~login();

private slots:
    void on_label_login_clicked();

//    void on_pushButton_2_clicked();

    void on_label_regist_clicked();

    void on_label_login_hovered();

    void on_label_login_unhovered();

    void on_label_regist_hovered();

    void on_label_regist_unhovered();

    void on_label_login_submit();

private:
    Ui::login *ui;
    registration res;
    MainWindow w;
};

#endif // LOGIN_H
