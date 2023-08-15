#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mqtt/qmqtt_message.h"
#include <QDebug>
#include <QProcess>
#include <QSerialPort>
#include <QUrlQuery>
#include <QNetworkRequest>

#if 1
    // 移动短信接口
QString _sdkAppId = "qq21497936";
QString _sdkAppKey = "f16095a81858314a3dbe16fdc4ec04d9";
#endif

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

void MainWindow::sendSMS(const QString &mobile, const QString &content)
{
    QString baseUrl = QString("https://api.yonyoucloud.com/apis/dst/mobilemessage/sendmessage");

    QUrl url;
    url.setUrl(baseUrl);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("ac", "send");
    // 笔者平台账户
    urlQuery.addQueryItem("uid", _sdkAppId);
    // 笔者平台账户接口密码
    urlQuery.addQueryItem("pwd", _sdkAppKey);
    urlQuery.addQueryItem("template", "100006");
    urlQuery.addQueryItem("mobile", mobile);
    urlQuery.addQueryItem("content", QString("{\"code\":\"%1\"}").arg(content));
    url.setQuery(urlQuery);
    _manager.get(QNetworkRequest(url));

//    QSerialPort serial;
//    serial.setPortName("/dev/ttyUSB0"); // 根据实际情况修改串口设备路径
//    serial.setBaudRate(QSerialPort::Baud115200); // 设置波特率
//    serial.setDataBits(QSerialPort::Data8);
//    serial.setParity(QSerialPort::NoParity);
//    serial.setStopBits(QSerialPort::OneStop);
//    serial.setFlowControl(QSerialPort::NoFlowControl);

//    if (!serial.open(QIODevice::ReadWrite))
//    {
//        qDebug() << "无法打开串口!\n"<<serial.error();
//        return;
//    }

//    // 发送AT指令初始化GSM模块
//    serial.write("AT\r\n");
//    serial.waitForBytesWritten(-1);
//    serial.waitForReadyRead(-1);
//    QByteArray response = serial.readAll();
//    qDebug() << "初始化响应：" << response;

//    // 设置短信格式为PDU模式
//    serial.write("AT+CMGF=0\r\n");
//    serial.waitForBytesWritten(-1);
//    serial.waitForReadyRead(-1);
//    response = serial.readAll();
//    qDebug() << "设置短信格式响应：" << response;

//    // 构建PDU格式的短信内容
//    QByteArray pdu = QString("0011000B81%1F00000AA1%2").arg(phoneNumber).arg(message.toLatin1().toHex()).toUpper().toUtf8();
//    int pduLength = pdu.length() / 2;

//    // 发送短信
//    serial.write(QString("AT+CMGS=%1\r\n").arg(pduLength).toUtf8());
//    serial.waitForBytesWritten(-1);
//    serial.waitForReadyRead(-1);
//    response = serial.readAll();
//    qDebug() << "发送短信响应：" << response;

//    serial.write(pdu + '\x1A'); // 发送短信内容和结束符
//    serial.waitForBytesWritten(-1);
//    serial.waitForReadyRead(-1);
//    response = serial.readAll();
//    qDebug() << "发送短信结果：" << response;

//    serial.close();
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
    const QString phonenumber="15086759102";
    const QString message="hello";
    sendSMS(phonenumber, message);
}
