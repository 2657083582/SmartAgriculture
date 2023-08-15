#include "registration.h"
#include "ui_registration.h"
#include <QMessageBox>

registration::registration(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::registration)
{
    ui->setupUi(this);
    logindata = new loginData();
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);

}

registration::~registration()
{
    delete ui;
    delete logindata;
}

void registration::on_pushButton_clicked()
{
    if(ui->lineEdit_2->text() != ui->lineEdit_3->text()){
        QMessageBox::warning(this,"警告","两次输入的密码不一致，请重新输入");
        return;
    }
    bool isUser = logindata->insert(ui->lineEdit->text(), ui->lineEdit_2->text());
    if(isUser == false) {
        QMessageBox::warning(this, "警告", "该用户名已存在。");
    } else {
        qDebug() << "创建成功";
        this->close();
        w.show();
    }
}

