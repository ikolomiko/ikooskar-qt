#ifndef IKOOSKAR_DAL_DATABASE_H
#define IKOOSKAR_DAL_DATABASE_H
#include "Shared/student.h"
#include <QList>
#include <QtSql>

namespace ikoOSKAR {
using namespace Shared;
namespace DAL {

    class Database {
    public:
        Database(QString& errorMessage);
        QHash<int, Student*>* GetAllStudents();
        bool Add(Student& s, QString& errorMessage);
        bool Update(Student& s, int oldId, QString& errorMessage);
        bool Delete(int id, QString& errorMessage);
        bool EndOfTheYear(QString& errorMessage);
        ~Database();

    private:
        void CreateDatabasePath();
        bool CreateDatabase(QString& errorMessage);
        bool DatabaseFileExists();
        QString GetDatabasePath(bool withFileName = false);

        QString databaseFilePath;
        QString databasePath;
    };

} // namespace DAL
} // namespace ikoOSKAR

#endif // IKOOSKAR_DAL_DATABASE_H
