#include "database.h"
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QtSql>
#include <QDebug>

namespace ikoOSKAR { namespace DAL {
    Database::Database(){
        this->databasePath = GetDatabasePath(false);
        this->databaseFilePath = GetDatabasePath(true);

        this->database = QSqlDatabase::addDatabase("QSQLITE");
        this->database.setDatabaseName(databaseFilePath);
        this->database.open();
        this->CreateDatabase();
    }

    bool Database::Add(Ogrenci const *o){
        QSqlQuery q;
        q.prepare("INSERT INTO ogrenciler (okulno, ad, soyad, sinif, sube) "
                  "VALUES (:okulno, :ad, :soyad, :sinif, :sube)");
        q.bindValue(":okulno", o->OkulNo);
        q.bindValue(":ad", o->Ad);
        q.bindValue(":soyad", o->Soyad);
        q.bindValue(":sinif", o->Sinif);
        q.bindValue(":sube", o->Sube);

        if(q.exec())
            return true;

        qDebug() << "Error adding new student "
                     << q.lastError();
        return false;

    }

    bool Database::Delete(Ogrenci *o){
        QSqlQuery q;
        q.prepare("DELETE FROM ogrenciler WHERE id = (:id)");
        q.bindValue(":id", o->Id);

        if(q.exec())
            return true;

        qDebug() << "Error deleting student "
                 << q.lastError();
        return false;
    }

    QList<Ogrenci> *Database::GetAllStudents(){
        QList<Ogrenci>* ogrenciler = new QList<Ogrenci>();
        QSqlQuery q("SELECT * FROM ogrenciler");
        while (q.next()) {
            Ogrenci *o = new Ogrenci();
            o->Id = q.value(0).toInt();
            o->Ad = q.value(1).toString();
            o->Soyad = q.value(2).toString();
            o->OkulNo = q.value(3).toInt();
            o->Sinif = q.value(4).toInt();
            o->Sube = q.value(5).toString();

            ogrenciler->append(*o);
        }
        return ogrenciler;
    }

    bool Database::Update(Ogrenci *o){
        QSqlQuery q;
        q.prepare("UPDATE ogrenciler SET okulno=(:okulno), ad=(:ad), soyad=(:soyad), sinif=(:sinif), sube=(:sube) WHERE id=(:id)");
        q.bindValue(":id", o->Id);
        q.bindValue(":okulno", o->OkulNo);
        q.bindValue(":ad", o->Ad);
        q.bindValue(":soyad", o->Soyad);
        q.bindValue(":sinif", o->Sinif);
        q.bindValue(":sube", o->Sube);

        if(q.exec())
            return true;

        qDebug() << "Error updating student "
                 << q.lastError();
        return false;
    }

    bool Database::CreateDatabase(){
        QSqlQuery q;
        q.prepare("CREATE TABLE IF NOT EXISTS ogrenciler ("
                  "id integer PRIMARY KEY AUTOINCREMENT,"
                  "ad text,"
                  "soyad text,"
                  "okulno integer NOT NULL,"
                  "sinif integer,"
                  "sube text )");
        if (q.exec())
            return true;

        qDebug() << "Error creating new database"
                 << q.lastError();
        return false;
    }

    QString Database::GetDatabasePath(bool withFileName){
        QString dirName = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        if (!withFileName) return dirName;

        QChar seperator = dirName[0] == '/' ? '/' : '\\';
        dirName.append(seperator);
        dirName.append("ogrenciler.db");
        return dirName;
    }

    bool Database::EndOfTheYear() {
        QSqlQuery q;
        QString cmds[] = {
            "DELETE FROM ogrenciler WHERE sinif=12",
            "UPDATE ogrenciler SET sinif=12 WHERE sinif=11"
            "UPDATE ogrenciler SET sinif=11 WHERE sinif=10"
            "UPDATE ogrenciler SET sinif=10 WHERE sinif=9"
        };

        // TODO

        //q.prepare();
    }

    Database::~Database(){
        this->database.close();
    }

}  // namespace DAL
} // namespace ikoOSKAR
