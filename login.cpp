#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

login::login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    logindata = new loginData();
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    res.setParent(this);
    res.hide();
}

login::~login()
{
    delete ui;
    delete logindata;
}

void login::on_pushButton_clicked()
{
    bool isUser = logindata->queryUser(ui->lineEdit->text(), ui->lineEdit_2->text());
    if(isUser == false) {
        QMessageBox::warning(this, "警告", "用户名和密码不匹配，请重新输入。");
    } else {
        w.show();
        this->close();
    }
}


void login::on_pushButton_2_clicked()
{
//    this->hide();
    res.show();
    ui->centralwidget->hide();
}
