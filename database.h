#ifndef DATABASE_H
#define DATABASE_H
#include <QString>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>


class DataBase
{
public:
    DataBase(QString dataBaseName);

    bool open();
    void close();
    bool qCreateTable(QString tableName, QStringList fields);
    bool qCreateTempTable(QString tableName, QStringList fields);
    bool qInsert(QString tableName, QStringList fields, QStringList values);
    bool qDropTable(QString tableName);
    bool insertIntoSelect(QString sourseTable, QString destTable);
    bool qSelectAll(QString tableName, int limit);
    bool nextSelect();
    QSqlRecord recordSelect();

private:
//    DataBase(QString dataBaseName);
//    DataBase(const DataBase&) = delete;
//    DataBase& operator = (const DataBase&) = delete;
    QSqlDatabase *DB;
    QSqlQuery *qSelect;

    static QMap<QString, DataBase*> pointerMap;
};

#endif // DATABASE_H
