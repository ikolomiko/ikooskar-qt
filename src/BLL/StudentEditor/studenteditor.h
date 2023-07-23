#ifndef IKOOSKAR_BLL_STUDENTEDITOR_H
#define IKOOSKAR_BLL_STUDENTEDITOR_H

#include "BLL/DatabaseHelper/databasehelper.h"
#include "Shared/student.h"

namespace ikoOSKAR {
namespace BLL {

    class StudentEditor : public QObject {
        Q_OBJECT
    private:
        DatabaseHelper* db;

        bool checkId(int id);
        bool checkName(QString& name);

    signals:
        void error(const QString& errorMessage);

    public:
        StudentEditor();
        bool createStudent(int id, QString& firstName, QString& lastName, int grade, QString& section);
        bool updateStudent(Shared::Student& original, int id, QString& firstName, QString& lastName, int grade, QString& section);
    };

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_STUDENTEDITOR_H
