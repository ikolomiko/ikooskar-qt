#ifndef IKOOSKAR_DAL_DATABASE_H
#define IKOOSKAR_DAL_DATABASE_H
#include <QList>
#include "Shared/student.h"
#include <QtSql>

namespace ikoOSKAR {
using namespace Shared;
namespace DAL {

class Database
{
public:
    Database();
    QHash<int, Student>* GetAllStudents();
    bool Add(Student *s, QString* errorMessage);
    bool Update(Student *s, QString* errorMessage);
    bool Delete(Student *s, QString* errorMessage);
    bool EndOfTheYear();
    QString GetDatabasePath(bool withFileName = false);
    bool DatabaseFileExists();
    void CreateDatabasePath();
    ~Database();

private:
    bool CreateDatabase();
    QString databaseFilePath;
    QString databasePath;
};

} // namespace DAL
} // namespace ikoOSKAR

#endif // IKOOSKAR_DAL_DATABASE_H
