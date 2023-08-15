#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mqtt/qmqtt_message.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    host=QHostAddress(ipaddr);
    client=new QMQTT::Client(host,port,nullptr);
    client->setKeepAlive(120);
    client->connectToHost();

    timer=new QTimer();
    timer->setInterval(1000*60);
    timer->start();

    connect(client, &QMQTT::Client::connected, client, [&](){
        qDebug()<<"connected";
        client->subscribe(subscribetopic);
    });


    connect(client, &QMQTT::Client::received, client, [&](const QMQTT::Message &message){
//        QString topic = message.topic();
        QByteArray payload = message.payload();
        QString str = QString::fromUtf8(payload);
        strlist = str.split(",");
        if(strlist.length() == 5){
            change_prperties(strlist);
        }


    });

    connect(timer, &QTimer::timeout, this, [&](){
        if(strlist.length() == 5){
            insertData(strlist);
        }else{
            qDebug()<<"error";
        }

    });




    on_ctl_auto_button_clicked();
//    QStringList testlist={"20","23","14","ON","OFF"};
//    insertData(testlist);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_ctl_pump_button_clicked()
{
    QByteArray data="pump";

    QMQTT::Message msg(0,publishtopic,data);
    client->publish(msg);
}


void MainWindow::on_ctl_fan_button_clicked()
{
    QByteArray data="fan";

    QMQTT::Message msg(0,publishtopic,data);
    client->publish(msg);
}



void MainWindow::on_ctl_auto_button_clicked()
{
    QString text = this->ui->ctl_auto_button->text();
    text = (text == "自动") ? "手动" : "自动";

    bool isAuto = (text == "手动");
    QByteArray data = (!isAuto) ? "manual":"auto";

    this->ui->ctl_fan_button->setEnabled(!isAuto);
    this->ui->ctl_pump_button->setEnabled(!isAuto);
    this->ui->ctl_auto_button->setText(text);

    QMQTT::Message msg(0,publishtopic,data);
    client->publish(msg);
}

void MainWindow::change_prperties(QStringList strlist)
{
    QString fanState=(strlist[2]=="ON")?"开":"关";
    QString pumpState=(strlist[4]=="ON")?"开":"关";
    this->ui->temp_label->setText(QString("环境温度:"+strlist[0]+"℃"));
    this->ui->humi_label->setText(QString("环境湿度:"+strlist[1]+"%"));
    this->ui->fan_state->setText(QString("风扇:"+fanState));
    this->ui->soil_label->setText(QString("土壤湿度:"+strlist[3]+"%"));
    this->ui->pump_state->setText(QString("水泵:"+pumpState));
}

void MainWindow::insertData(QStringList strlist)
{
    QString time=QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
    float airtemp=strlist[0].toFloat();
    float airhumi=strlist[1].toFloat();
    QString fanstate=strlist[2];
    float soilhumi=strlist[3].toFloat();
    QString pumpstate=strlist[4];
    Data data(time,airtemp,airhumi,soilhumi,fanstate,pumpstate);
    sql.insert(data);
}

void MainWindow::saveFile(QString path,QList<Data> datalist)
{
    QString baseFileName = "smartagriculture";
    QString fileExtension = ".txt";

    QString newFileName = baseFileName;
    int index = 1;

    // 创建一个QFile对象，以只写模式打开文件
    QDir directory(path);
    QFile file(directory.filePath(newFileName + fileExtension)); // 使用目录路径和文件名生成完整的文件路径

    while (file.exists()) {
        // 如果文件已存在，根据索引生成新的文件名
        newFileName = baseFileName + QString::number(index);
        file.setFileName(directory.filePath(newFileName + fileExtension)); // 使用目录路径和新的文件名生成完整的文件路径
        index++;
    }

    if (file.open(QIODevice::WriteOnly)) {
        // 文件成功打开，可以在这里进行写入操作
        for(const auto &data:datalist){
           QString at=QString::number(data.AirTemp);
           QString ah=QString::number(data.AirHumi);
           QString sh=QString::number(data.SoilHumi);
           QString write_data=data.time+","+at+","+ah+","+
                                sh+","+data.fanState+","+data.pumpState+"\n";
           QByteArray byteArray(write_data.toUtf8());
           file.write(byteArray);
        }
        file.close(); // 关闭文件

        QMessageBox::information(this,"提示","文件保存成功！",QMessageBox::Yes,QMessageBox::Yes);
    } else {
        // 文件打开失败，处理错误
        qDebug() << "无法创建文件：" << file.errorString();
    }
}

void MainWindow::on_test_button_clicked()
{
    QList<Data> datalist=sql.queryAll();
    for(const auto &data:datalist){
        qDebug()<<data.time<<","<<data.AirTemp<<","<<data.AirHumi<<","<<data.SoilHumi<<","<<data.fanState<<","<<data.pumpState;
    }

    QString directoryPath = QFileDialog::getExistingDirectory(this, "选择文件夹", "", QFileDialog::ShowDirsOnly);

    if (!directoryPath.isEmpty()) {
        qDebug() << "选择的文件夹路径：" << directoryPath;
        saveFile(directoryPath,datalist);
    } else {
        // 用户取消了文件夹选择
        qDebug() << "取消选择文件夹";
    }
}

