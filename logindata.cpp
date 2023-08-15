#include "logindata.h"

loginData::loginData()
{
    initLoginData();
    createTable();
}

bool loginData::initLoginData()
{
    QSqlDatabase db;

    if(QSqlDatabase::contains("qt_sql_default_connection")){
        db = QSqlDatabase::database("qt_sql_default_connection");
    }else{
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName("127.0.0.1");
        db.setDatabaseName("loginData.db");
        db.setUserName("root");
        db.setPassword("123");
    }

    if(!db.open()){
        qDebug()<<"open database failed";
        return false;
    }

    return true;

}

bool loginData::createTable()
{
    QSqlDatabase database = QSqlDatabase::database();
    if (database.tables().contains("user")) {
        // 表已经存在，无需创建
        return true;  // 返回创建成功，因为表已存在
    } else {
        QSqlQuery query;
        QString query_string="CREATE TABLE user(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                               "userName REAL NOT NULL,"
                               "password REAL NOT NULL)";
        query.prepare(query_string);
        if(query.exec(query_string)){
            qDebug()<<"create user successed";
            return true;
        }else{
            qDebug()<<query.lastError();
            return false;
        }
    }

}

bool loginData::queryUser(QString userName, QString password)
{
    QSqlQuery query;
    QString query_string=QString("SELECT password FROM user where userName = '%1'").arg(userName);
    query.prepare(query_string);
//    query.addBindValue(userName);
    query.exec(query_string);
    if(query.next()) {
        QString fetchedPassword = query.value(0).toString(); // 获取查询结果中的密码值
        if (fetchedPassword == password){
            return true;
        }
    }
    return false;
}

bool loginData::insert(QString userName, QString password)
{
    QSqlQuery query;

    QString query_string=QString("SELECT userName FROM user where userName = '%1'").arg(userName);
    query.prepare(query_string);
//    query.addBindValue(userName);
    query.exec(query_string);
    if(query.next()) {
        QString fetchedUserName = query.value(0).toString(); // 获取查询结果中的密码值
        if (fetchedUserName == userName){
            return false;
        }
    }

    query_string = QString("INSERT INTO user(userName,password) values('%1','%2')").arg(userName).arg(password);
    if(!query.exec(query_string)){
        qDebug() << query.lastError();
        return false;
    }
    return true;
}
