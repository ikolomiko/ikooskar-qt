#ifndef IKOOSKAR_BLL_DATABASEHELPER_H
#define IKOOSKAR_BLL_DATABASEHELPER_H

#include "DAL/Database/database.h"
#include "Shared/student.h"
#include "UI/ErrorUi/errorui.h"
#include <QList>

namespace ikoOSKAR {
namespace BLL {

    /// @brief The DatabaseHelper class handles all the basic database operations and handles errors.
    class DatabaseHelper {
    private:
        /**
         * @brief Holds a pointer to a common {@code ikoOSKAR::DAL::Database} instance
         * @see ikoOSKAR::DAL::Database
         */
        ikoOSKAR::DAL::Database* dal;

        /**
         * @brief Holds a cache of the database in the {@code QHash<int, Student>} format, where int is equal to {@code Student.Id}
         * @see ikoOSKAR::Shared::Student
         */
        QHash<int, Student>* databaseCache;

        /**
         * @brief Holds a pointer to a common ErrorUi instance with a previously determined title
         * @see ikoOSKAR::UI::ErrorUi
         */
        UI::ErrorUi* errorUi;

    public:
        // DatabaseHelper(std::function<void(const QString& message)> errHandler);
        DatabaseHelper(UI::ErrorUi*);
        void Add(Student*);
        void Update(Student*, int);
        void UpdateWithNewId(int oldId, Student*);
        void Delete(int id);
        bool IdExists(int id);
        QList<int> GetAllIds();
        void EndOfTheYear();
        QList<QString> GetClassNames();
        QList<Student*> GetStudentsByClassName(int grade, QString& section);
        QList<Student*> GetStudentsByClassName(QString& className);
        Student* GetStudentById(int id);
        bool CheckForPrerequsities();
        ~DatabaseHelper();
    };

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_DATABASEHELPER_H
