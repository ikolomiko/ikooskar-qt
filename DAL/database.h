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
    QList<Student>* GetAllStudents();
    bool Add(Student const *o);
    bool Update(Student*);
    bool Delete(Student*);
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
