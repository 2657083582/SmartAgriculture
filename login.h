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
    ~login();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::login *ui;
    registration res;
    MainWindow w;
};

#endif // LOGIN_H
