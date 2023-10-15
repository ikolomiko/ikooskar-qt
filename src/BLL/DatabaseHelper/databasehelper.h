#ifndef IKOOSKAR_BLL_DATABASEHELPER_H
#define IKOOSKAR_BLL_DATABASEHELPER_H

#include "DAL/Database/database.h"
#include <QList>

namespace ikoOSKAR {
namespace BLL {

    /// @brief The DatabaseHelper class handles all the basic database operations and handles errors.
    class DatabaseHelper : public QObject {
        Q_OBJECT
    private:
        /**
         * @brief Holds a pointer to a common {@code ikoOSKAR::DAL::Database} instance
         * @see ikoOSKAR::DAL::Database
         */
        ikoOSKAR::DAL::Database* dal;

        /**
         * @brief Holds a cache of the students in the database in the {@code QHash<int, Student>} format, where int is equal to {@code Student.id}
         * @see ikoOSKAR::Shared::Student
         */
        QHash<int, Student*>* studentCache;

        /**
         * @brief Holds a cache of the exam halls in the database in the {@code QHash<QString, Student>} format, where QString is equal to {@code Hall.name}
         * @see ikoOSKAR::Shared::Hall
         */
        QHash<QString, Hall*>* hallCache;

        DatabaseHelper();
        QString turkishToUpper(const QString& s);
        QString turkishToLower(const QString& s);
        QString* formatForFirstName(const QString& raw);
        QString* formatForLastName(const QString& raw);

        inline static DatabaseHelper* instance;

    signals:
        void error(const QString& errorMessage);

    public:
        static DatabaseHelper* getInstance();
        static void sortClassnames(QList<QString>* classNames);
        static QPair<int, QString> ParseClassName(const QString& className);
        void Add(Student&);
        void Add(Hall&);
        void AddAll(QList<Student*>&);
        void Update(Student&, int oldId);
        void Update(Hall&, const QString& oldName);
        void Delete(int id);
        void Delete(const QString& hallName);
        bool IdExists(int id);
        bool HallNameExists(const QString& hallName);
        QList<int> GetAllIds();
        QList<QString> GetAllHallNames();
        QList<Hall*>* GetHallsByName(const QList<QString>& hallNames);
        void EndOfTheYear();
        QList<QString>* GetClassNames();
        QList<Student*>* GetStudentsByClassName(int grade, const QString& section);
        QList<Student*>* GetStudentsByClassName(const QString& className);
        Student* GetStudentById(int id);
        Hall* GetHallByName(const QString& hallName);
        int GetNumberOfStudents();
        void DeleteEntireClass(const QString& className);
        void AddDesk(Hall&, int n);
        void RemoveDesk(Hall&, int n);
        void ResizeClassroom(int grade, const QString& section);
        void ResizeAllClassrooms();
        ~DatabaseHelper();
    };

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_DATABASEHELPER_H
