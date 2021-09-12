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
    Database();
    QList<Ogrenci>* GetAllStudents();
    bool Add(Ogrenci const *o);
    bool Update(Ogrenci*);
    bool Delete(Ogrenci*);
    bool EndOfTheYear();
    QString GetDatabasePath(bool withFileName = false);
    ~Database();

private:
    bool CreateDatabase();
    QString databaseFilePath;
    QString databasePath;
};

} // namespace DAL
} // namespace ikoOSKAR

#endif // IKOOSKAR_DAL_DATABASE_H
