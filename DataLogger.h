#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QtSql>
#include <QString>

class SystemData
{
public:

};

class DataLogger
{
public:
    DataLogger(QString& path) : dbPath(path) {};
    // bool openDB();

private:
    QString& dbPath;
    QSqlDatabase database;
};

#endif // DATALOGGER_H
