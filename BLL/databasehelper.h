#ifndef IKOOSKAR_BLL_DATABASEHELPER_H
#define IKOOSKAR_BLL_DATABASEHELPER_H

#include <QList>
#include "Shared/student.h"
#include "DAL/database.h"

namespace ikoOSKAR {
namespace BLL {

class DatabaseHelper
{    
private:
    QHash<int, Student>* databaseCache;
    ikoOSKAR::DAL::Database* dal;
    std::function<void(QString message)> handleError;

public:
    DatabaseHelper(std::function<void(QString message)> errHandler);
    void RefreshCache();
    void Add(Student*);
    void Update(Student*);
    void UpdateWithNewId(int oldId, Student*);
    void Delete(Student*);
    bool IdExists(int id);
    QList<int> GetAllIds();
    void EndOfTheYear();
    QList<QString> GetClassNames();
    QList<Student*> GetStudentsByClassName(int grade, QString section);
    Student* GetStudentById(int id);
    bool CheckForPrerequsities(); ///TODO
    ~DatabaseHelper();
};

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_DATABASEHELPER_H
