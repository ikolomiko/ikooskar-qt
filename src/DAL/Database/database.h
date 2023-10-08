#ifndef IKOOSKAR_DAL_DATABASE_H
#define IKOOSKAR_DAL_DATABASE_H
#include "Shared/hall.h"
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
        QHash<QString, Hall*>* GetAllHalls();
        bool Add(Student& s, QString& errorMessage);
        bool Add(Hall& h, QString& errorMessage);
        bool Update(Student& s, int oldId, QString& errorMessage);
        bool Update(Hall& h, const QString& oldName, QString& errorMessage);
        bool Delete(int id, QString& errorMessage);
        bool Delete(const QString& hallName, QString& errorMessage);
        bool EndOfTheYear(QString& errorMessage);
        static QString GetDatabasePath(bool withFileName = false);
        ~Database();

    private:
        void CreateDatabasePath();
        bool CreateDatabase(QString& errorMessage);
        bool DatabaseFileExists();

        QString databaseFilePath;
        QString databasePath;
    };

} // namespace DAL
} // namespace ikoOSKAR

#endif // IKOOSKAR_DAL_DATABASE_H
