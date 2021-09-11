#include "database.h"
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QtSql>

namespace ikoOSKAR { namespace DAL {
    Database::Database(QString *result){
        this->databasePath = GetDatabasePath(false);
        this->databaseFilePath = GetDatabasePath(true);

        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(databaseFilePath);
        database.open();
        this->database = database;
    }

    void Database::Add(Ogrenci o){

    }

    void Database::Delete(Ogrenci o){

    }

    QList<Ogrenci> Database::GetAllStudents(){
        return QList<Ogrenci>();
    }

    void Database::Update(Ogrenci o){

    }

    bool Database::DatabaseExists(){
        return QFile::exists(this->databaseFilePath);
    }

    void Database::CreateDatabase(){
        if(!QDir(this->databasePath).exists())
            QDir().mkdir(this->databasePath);

        QFile file(databaseFilePath);
        file.open(QIODevice::ReadWrite);

        file.write("deneme2\n");
        file.close();
    }

    QString Database::GetDatabasePath(bool withFileName){
        QString dirName = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        if (!withFileName) return dirName;

        QChar seperator = dirName[0] == '/' ? '/' : '\\';
        dirName.append(seperator);
        dirName.append("ogrenciler.db");
        return dirName;
    }

    Database::~Database(){
        this->database.close();
    }

}  // namespace DAL
} // namespace ikoOSKAR
