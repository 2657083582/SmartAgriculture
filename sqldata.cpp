#include "sqldata.h"

Sqldata::Sqldata()
{
    initDataBase();
}

Sqldata::~Sqldata()
{
//    dropTable();
}

bool Sqldata::insert(Data data)
{
    QSqlQuery query;
    QString query_string=QString("INSERT INTO agriculture(time,airTemp,airHumi,soilHumi,fanState,pumpState) "
                                 "values('%1','%2','%3','%4','%5','%6')").arg(data.time).arg(data.AirTemp).arg(data.AirHumi).arg(data.SoilHumi).arg(data.fanState).arg(data.pumpState);
    if(!query.exec(query_string)){
        qDebug()<<query.lastError();
        return false;
    }
    return true;
}

QList<Data> Sqldata::queryAll()
{
    QSqlQuery query;
    QString query_string=QString("SELECT * FROM agriculture");
    query.prepare(query_string);
    query.exec(query_string);
    QList<Data> datalist;
    while(query.next()){
        QString time=query.value(0).toString();
        float airTemp=query.value(1).toFloat();
        float airHumi=query.value(2).toFloat();
        float soilHumi=query.value(3).toFloat();
        QString fanState=query.value(4).toString();
        QString pumpState=query.value(5).toString();
        Data *data=new Data(time,airTemp,airHumi,soilHumi,fanState,pumpState);
        datalist.append(*data);
        delete data;
    }
    return datalist;
}

bool Sqldata::initDataBase()
{
    QSqlDatabase db;
    //包含默认连接则使用默认连接，否则按照以下信息设置数据库
    if(QSqlDatabase::contains("qt_sql_default_connection")){
        db = QSqlDatabase::database("qt_sql_default_connection");
    }else{
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName("127.0.0.1");
        db.setDatabaseName("SmartAgricultureDb.db");
        db.setUserName("root");
        db.setPassword("123");
    }
    if(!db.open()){
        qDebug()<<"open database failed";
        return false;
    }
    if(!db.tables().contains("agriculture")){
        if(!createTable())
            return false;
    }

    return true;
}

bool Sqldata::createTable()
{
    QSqlQuery query;
    QString query_string="CREATE TABLE agriculture(time TEXT PRIMARY KEY,"
                         "airTemp REAL NOT NULL,"
                         "airHumi REAL NOT NULL,"
                         "soilHumi REAL NOT NULL,"
                         "fanState TEXT NOT NULL,"
                         "pumpState TEXT NOT NULL)";
    query.prepare(query_string);
    if(query.exec(query_string)){
        qDebug()<<"create agriculture successed";
        return true;
    }else{
        qDebug()<<query.lastError();
        return false;
    }
}

bool Sqldata::dropTable()
{
    QSqlQuery query;
    QString query_string("DROP TABLE IF EXISTS agriculture");
    if(query.exec(query_string)){
        qDebug()<<"drop table success";
    }else{
        qDebug()<<query.lastError();
        return false;
    }
    return true;
}
