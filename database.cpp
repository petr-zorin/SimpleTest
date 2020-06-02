#include "database.h"

DataBase::DataBase(QString dataBaseName)
{
    DB = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    if(DB) DB->setDatabaseName(dataBaseName);
    qSelect = new QSqlQuery(*DB);
}

bool DataBase::open()
{
    return DB->open();
}

void DataBase::close()
{
    DB->close();
}

bool DataBase::qCreateTable(QString tableName, QStringList fields) //Создание таблицы <имя таблицы>, <список полей>
{
    QString str = "CREATE TABLE %1("
                  "id INTEGER,"
                  "%2 VARCHAR(20),"
                  "%3 VARCHAR(4),"
                  "%4 VARCHAR(50),"
                  "%5 VARCHAR(50))";
    QString queryStr = str.arg(tableName).arg(QString(fields.at(1))).arg(QString(fields.at(2))).arg(QString(fields.at(3))).arg(QString(fields.at(4)));
    QSqlQuery query(*DB);
    query.prepare(queryStr);
    return query.exec();
}

bool DataBase::qCreateTempTable(QString tableName, QStringList fields) //Создание временной таблицы <имя таблицы>, <список полей>
{                                                                      //Этот метод можно объединить с предыдущим методом
    QString str = "CREATE TEMP TABLE %1("
                  "id INTEGER,"
                  "%2 VARCHAR(20),"
                  "%3 VARCHAR(4),"
                  "%4 VARCHAR(50),"
                  "%5 VARCHAR(50))";
    QString queryStr = str.arg(tableName).arg(QString(fields.at(1))).arg(QString(fields.at(2))).arg(QString(fields.at(3))).arg(QString(fields.at(4)));
    QSqlQuery query(*DB);
    query.prepare(queryStr);
    return query.exec();
}

bool DataBase::qInsert(QString tableName, QStringList fields, QStringList values)   //Вставка записей в <имя таблицы>, <список полей>, <список значений>
{
    QString str="INSERT INTO %1 (%2, %3, %4, %5, %6) VALUES(%7, '%8', '%9', '%10', '%11')";
    QString queryStr = str .arg(tableName)
                        .arg(fields.at(0))         .arg(fields.at(1)).arg(fields.at(2)).arg(fields.at(3)).arg(fields.at(4)) //имена полей
                        .arg(values.at(0)).arg(values.at(1)).arg(values.at(2)).arg(values.at(3)).arg(values.at(4));         //значения

    QSqlQuery query(*DB);
    query.prepare(queryStr);
    return query.exec();
}

bool DataBase::qDropTable(QString tableName) //удаление таблицы
{
    QString str = "DROP TABLE IF EXISTS %1";
    QString queryStr = str.arg(tableName);
    QSqlQuery query(*DB);
    query.prepare(queryStr);
    return query.exec();
}

bool DataBase::insertIntoSelect(QString sourseTable, QString destTable) //копирование данных между таблицами
{
    QString str = "INSERT INTO %1 SELECT * FROM %2";
    QString queryStr = str.arg(destTable).arg(sourseTable);
    QSqlQuery query(*DB);
    query.prepare(queryStr);
    return query.exec();
}

bool DataBase::qSelectAll(QString tableName, int limit) //Выборка первых <limit> записей
{
    QString str = "SELECT * FROM %1 LIMIT %2";
    QString queryStr = str.arg(tableName).arg(limit);

    qSelect->prepare(queryStr);
    return qSelect->exec();
}

bool DataBase::nextSelect()                             //Переход на следующую запись запроса SELECT
{
    return qSelect->next();
}

QSqlRecord DataBase::recordSelect()                     //Получение данных записи из SELECT
{
    return qSelect->record();
}
