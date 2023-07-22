#ifndef IKOOSKAR_BLL_STUDENTEDITOR_H
#define IKOOSKAR_BLL_STUDENTEDITOR_H

#include "BLL/DatabaseHelper/databasehelper.h"
#include "Shared/student.h"
#include "UI/ErrorUi/errorui.h"

namespace ikoOSKAR {
namespace BLL {

    class StudentEditor {
    private:
        UI::ErrorUi* errorUi;
        DatabaseHelper* db;

        bool checkId(int id);
        bool checkName(QString& name);

    public:
        StudentEditor(UI::ErrorUi* errorUi);
        bool createStudent(int id, QString& firstName, QString& lastName, int grade, QString& section);
        bool updateStudent(Shared::Student& original, int id, QString& firstName, QString& lastName, int grade, QString& section);
    };

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_STUDENTEDITOR_H
