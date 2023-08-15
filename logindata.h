#ifndef LOGINDATA_H
#define LOGINDATA_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class loginData
{
public:
    loginData();
    bool queryUser(QString userName, QString password);
    bool insert(QString userName, QString password);

private:
    bool initLoginData();
    bool createTable();
};

#endif // LOGINDATA_H
