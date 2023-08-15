#ifndef SQLDATA_H
#define SQLDATA_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTime>


struct Data{
    QString time;
    float AirTemp;
    float AirHumi;
    float SoilHumi;
    QString fanState;
    QString pumpState;

    Data(QString time,float at,float ah,float sh,QString fs,QString ps)
        :time{time},AirTemp{at},AirHumi{ah},SoilHumi{sh},fanState{fs},pumpState{ps}
    {
//        this->time = time.toString("yy-MM-dd hh:mm:ss");
//        qDebug()<<this->time;
    }

};

class Sqldata
{
public:
    Sqldata();
    ~Sqldata();

    bool insert(Data data);
    QList<Data> queryAll();


private:
    bool initDataBase();
    bool createTable();
    bool dropTable();


};

#endif // SQLDATA_H
