#ifndef IKOOSKAR_DAL_DATABASE_H
#define IKOOSKAR_DAL_DATABASE_H
#include <QList>
#include "Shared/ogrenci.h"
#include <QtSql>

namespace ikoOSKAR {
using namespace Shared;
namespace DAL {

class Database
{
public:
    Database(QString *result);
    QList<Ogrenci> GetAllStudents();
    void Add(Ogrenci);
    void Update(Ogrenci);
    void Delete(Ogrenci);
    bool DatabaseExists();
    void CreateDatabase();
    void EndOfTheYear();
    QString GetDatabasePath(bool withFileName = false);
    ~Database();

private:
    QString databaseFilePath;
    QString databasePath;
    QSqlDatabase database;
};

} // namespace DAL
} // namespace ikoOSKAR

#endif // IKOOSKAR_DAL_DATABASE_H
