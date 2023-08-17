#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

login::login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    logindata = new loginData();
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
    res.setParent(this);
    res.hide();
}

void login::setUser(QString userName, QString passWord)
{
    ui->lineEdit_userName->text() = userName;
    ui->lineEdit_password->text() = passWord;
}

login::~login()
{
    delete ui;
    delete logindata;
}

void login::on_label_login_clicked()
{
    if(ui->lineEdit_userName->text() == "" || ui->lineEdit_password->text() == "") {
        QMessageBox::warning(this, "警告", "用户名或密码不能为空!");
        return;
    }
    bool isUser = logindata->queryUser(ui->lineEdit_userName->text(), ui->lineEdit_password->text());
    if(isUser == false) {
        QMessageBox::warning(this, "警告", "用户名和密码不匹配，请重新输入!");
    } else {
        w.show();
        this->close();
    }
}

//void login::on_pushButton_2_clicked()
//{
//    this->hide();
//    res.show();
//    ui->centralwidget->hide();
//}

void login::on_label_regist_clicked()
{
    res.show();
    this->setFixedSize(388, 300);
    ui->centralwidget->hide();
}


void login::on_label_login_hovered()
{
    ui->label_login->setFrameShape(QFrame::Box);
    ui->label_login->setFrameShadow(QFrame::Sunken);
}


void login::on_label_login_unhovered()
{
    ui->label_login->setFrameShape(QFrame::NoFrame);
}


void login::on_label_regist_hovered()
{
    ui->label_regist->setFrameShape(QFrame::Box);
    ui->label_regist->setFrameShadow(QFrame::Sunken);
}


void login::on_label_regist_unhovered()
{
    ui->label_regist->setFrameShape(QFrame::NoFrame);
}

