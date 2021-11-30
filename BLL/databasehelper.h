#ifndef IKOOSKAR_BLL_DATABASEHELPER_H
#define IKOOSKAR_BLL_DATABASEHELPER_H

#include <QList>
#include "Shared/student.h"
#include "DAL/database.h"

using ikoOSKAR::Shared::Student;

namespace ikoOSKAR {
namespace BLL {

class DatabaseHelper
{
public:
    DatabaseHelper();
    void Add(Student*);
    void AddRange(QList<Student*>);
    void Update(Student*);
    void Delete(Student*);
    bool IdExists(int id);
    QList<int> GetAllIds();
    void EndOfTheYear();
    QList<QString> GetClassNames();
    QList<Student*> GetStudentsByClassName(int grade, QString section);
    Student* GetStudentById(int id);
    bool CheckForPrerequsities(); ///TODO
    ~DatabaseHelper();

private:
    QList<Student>* databaseCache;
    ikoOSKAR::DAL::Database* dal;

};

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_DATABASEHELPER_H
