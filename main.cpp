#include <QCoreApplication>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QFile>
#include "database.h"

void convertCsv(DataBase *db, QString fileName, QString tableName);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DataBase *db = nullptr; //создание объекта класса и открытие в нём базы данных
    db = new DataBase("data.db");
    if(db) {
        if(db->open())  qDebug()<<"Database opened";
        else qDebug()<<"Error, open database fail";
    }
    else qDebug()<<"Error, create db fail";

    convertCsv(db, "staff.csv", "staff");
    convertCsv(db, "info.csv", "info");

    db->close();
    delete db;
    exit(0);

}
void convertCsv(DataBase *db, QString fileName, QString tableName)
{
    QFile in1(fileName);
    if(in1.open(QIODevice::ReadOnly)) {
        QList<QByteArray> fields = in1.readLine().trimmed().split(';'); //чтение имён полей из файла
        qDebug() << endl << endl << "Converting file" << fileName <<"...";

        //Создание временной таблицы TempTable с именами полей из файла .csv
        if (db->qCreateTempTable("TempTable", {fields.at(0), fields.at(1), fields.at(2), fields.at(3), fields.at(4)}))
            qDebug() << "TempTable created";
        else qDebug() << "Error creation TempTable";

        while(!in1.atEnd()) {
            QList<QByteArray>tmp=in1.readLine().trimmed().split(';');

            if(db->qInsert("TempTable",         //запись строк из файла в TempTable
            {fields.at(0), fields.at(1), fields.at(2), fields.at(3), fields.at(4)}, //имена полей
            {tmp.at(0),    tmp.at(1),    tmp.at(2),    tmp.at(3),    tmp.at(4)}))   //вставляемые данные
            {
//                qDebug()<<"Insert success";
            }
            else {
                qDebug()<<"Insert fail";
            }
        }

        if (db->qDropTable(tableName))                        //удаление из базы таблицы tableName, если существует
            qDebug() << "Table" << tableName << "droped";
        else qDebug() << "Unable to drop table" << tableName;

        //создание новой таблицы tableName
        if (db->qCreateTable(tableName, {fields.at(0), fields.at(1), fields.at(2), fields.at(3), fields.at(4)}))
            qDebug() << "Table" << tableName <<" created";
        else qDebug() << "Unable to create " << tableName;

        if (db->insertIntoSelect("TempTable", tableName))     //копирование данных из временной таблицы TempTable в таблицу tableName
            qDebug() << "Data copied to" << tableName;
        else qDebug() << "Data wasn't copied to" << tableName;

        if (db->qDropTable("TempTable"))                        //удаление из базы таблицы TempTable
            qDebug() << "TempTable droped";
        else qDebug() << "Unable to drop TempTable";

        if (db->qSelectAll(tableName, 5))                   //проверка содержимого таблицы tableName, первые 100 строк
        {
            qDebug() << endl << "SELECT table" << tableName << "LIMIT 5:";
            while (db->nextSelect())
            {
                QSqlRecord rec = db->recordSelect();
                int id = rec.value(rec.indexOf(fields.at(0))).toInt();
                QString f1 = rec.value(rec.indexOf(fields.at(1))).toString();
                QString f2 = rec.value(rec.indexOf(fields.at(2))).toString();
                QString f3 = rec.value(rec.indexOf(fields.at(3))).toString();
                QString f4 = rec.value(rec.indexOf(fields.at(4))).toString();
                qDebug() << id << f1 << f2 << f3 << f4;
            }
        }
        else qDebug()<<"Request SELECT ALL fail";
        qDebug() << "The conversion"<< fileName <<"complited";
        in1.close();
    }
}
