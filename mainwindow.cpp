#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mqtt/qmqtt_message.h"
#include <QThread>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->lineEdit_temp->hide();
    this->ui->lineEdit_humi->hide();
    this->setWindowFlags(Qt::FramelessWindowHint);
//    this->setWindowOpacity(0.9);
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

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_ctl_pump_button_clicked()
{
    if(this->ui->ctl_pump_button->able())
        return;
    qDebug()<<"pump button clicked";
    bool flag=this->ui->ctl_pump_button->flag();

    QByteArray data="pump:1";
    //QByteArray data=flag?"pump:0":"pump:1";
    QMQTT::Message msg(0,publishtopic,data);
    client->publish(msg);

    this->ui->ctl_pump_button->set_flag(!flag);
    QString text = (!flag)?"开":"关";
    this->ui->pump_state->setText(text);
}


void MainWindow::on_ctl_fan_button_clicked()
{
    if(this->ui->ctl_fan_button->able())
        return;
    qDebug()<<"fan button clicked";
    bool flag=this->ui->ctl_fan_button->flag();

    QByteArray data="fan:1";
    //QByteArray data=flag?"fan:0":"fan:1";
    QMQTT::Message msg(0,publishtopic,data);
    client->publish(msg);

    this->ui->ctl_fan_button->change_flag();
    QString text = (!flag)?"开":"关";
    this->ui->fan_state->setText(text);
}

void MainWindow::change_prperties(QStringList strlist)
{
    QString fanState=(strlist[2]=="ON")?"开":"关";
    QString pumpState=(strlist[4]=="ON")?"开":"关";
    this->ui->temp_label->setText(QString(strlist[0]+"℃"));
    this->ui->humi_label->setText(QString(strlist[1]+"%RH"));
    this->ui->soil_label->setText(QString(strlist[3]+"%RH"));
    if(this->ui->ctl_fan_button->able()){
        this->ui->fan_state->setText(QString(fanState));
        this->ui->pump_state->setText(QString(pumpState));
    }
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

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->globalPosition();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(this->x()+event->globalPosition().x()-m_lastPos.x(),
               this->y()+event->globalPosition().y()-m_lastPos.y());
    m_lastPos = event->globalPosition();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_lastPos = event->globalPosition();
}

void MainWindow::on_export_data_label_clicked()
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



void MainWindow::on_quit_label_clicked()
{
    QApplication::quit();
}


void MainWindow::on_min_label_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}


void MainWindow::on_quit_label_hovered()
{
    this->ui->quit_label->setStyleSheet("background-color: rgb(211,230,222);"
                                        "image: url(:/退出.png);");
}


void MainWindow::on_quit_label_unhovered()
{
    this->ui->quit_label->setStyleSheet("background-color: rgb(180, 211, 206);"
                                        "image: url(:/退出.png);");
}


void MainWindow::on_min_label_hovered()
{
    this->ui->min_label->setStyleSheet("background-color: rgb(211,230,222);"
                                       "image: url(:/最小化.png)");
}


void MainWindow::on_min_label_unhovered()
{
    this->ui->min_label->setStyleSheet("background-color: rgb(180, 211, 206);"
                                       "image: url(:/最小化.png)");
}




void MainWindow::on_auto_label_clicked()
{
    bool able=!this->ui->ctl_fan_button->able();
    this->ui->ctl_fan_button->set_able(able);
    this->ui->ctl_pump_button->set_able(able);

    if(!able){
        this->ui->ctl_fan_button->set_flag(this->ui->fan_state->text() == "开");
        this->ui->ctl_pump_button->set_flag(this->ui->pump_state->text() == "开");
    }

    QString text=(able)?"自动":"手动";
    this->ui->auto_label->setText(text);

    QByteArray data = (able) ? "auto":"manual";
    QMQTT::Message msg(0,publishtopic,data);
    client->publish(msg);
}

void MainWindow::on_auto_label_hovered()
{
    ui->auto_label->setFrameShape(QFrame::Box);
    ui->auto_label->setFrameShadow(QFrame::Sunken);
}


void MainWindow::on_auto_label_unhovered()
{
    ui->auto_label->setFrameShape(QFrame::NoFrame);
}


void MainWindow::on_export_data_label_hovered()
{
    ui->export_data_label->setStyleSheet("image: url(:/导出数据.png);"
                                         "background-color: rgb(220, 205, 210);");
}


void MainWindow::on_export_data_label_unhovered()
{
    ui->export_data_label->setStyleSheet("image: url(:/导出数据.png);");
}


void MainWindow::on_temp_edit_clicked()
{
    ui->lineEdit_temp->show();
}




void MainWindow::on_lineEdit_temp_submit()
{
    QString temp=ui->lineEdit_temp->text();
    bool ok;
    double t=temp.toDouble(&ok);
    if(t >= 0 && t <= 100)
        ok &= true;
    else{
        ok &= false;
    }
    if(!ok){
        QMessageBox::warning(this,"修改失败","请输入0～100的数字！！！");
        return;
    }
    ui->temp_label_2->setText(temp+"°C");
    QString data="temp:"+temp;
    QByteArray byteArray=data.toUtf8();
    QMQTT::Message msg(0,publishtopic,byteArray);
    client->publish(msg);

    ui->lineEdit_temp->hide();
}


void MainWindow::on_lineEdit_temp_cancel()
{
    ui->lineEdit_temp->hide();
}


void MainWindow::on_lineEdit_humi_submit()
{
    QString humi = ui->lineEdit_humi->text();
    bool ok;
    double h = humi.toDouble(&ok);
    if(h >= 0 && h <= 100)
        ok &= true;
    else{
        ok &= false;
    }
    if(!ok){
        QMessageBox::warning(this,"修改失败","请输入0～100的数字！！！");
        return;
    }
    ui->humi_label_2->setText(humi+"%RH");
    QString data="humi:"+humi;
    QByteArray byteArray=data.toUtf8();
    QMQTT::Message msg(0,publishtopic,byteArray);
    ui->lineEdit_humi->hide();
}


void MainWindow::on_humi_edit_clicked()
{
    ui->lineEdit_humi->show();
}


void MainWindow::on_lineEdit_humi_cancel()
{
    ui->lineEdit_humi->hide();
}

