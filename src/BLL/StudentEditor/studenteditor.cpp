#include "studenteditor.h"
#include <QLocale>

namespace ikoOSKAR {
namespace BLL {

    StudentEditor::StudentEditor(UI::ErrorUi* errorUi)
        : errorUi(errorUi)
    {
        db = DatabaseHelper::getInstance(errorUi);
    }

    bool StudentEditor::checkId(int id)
    {
        if (id < 0) {
            errorUi->DisplayMessage("Okul no negatif bir sayı olamaz.");
            return false;
        }
        if (db->IdExists(id)) {
            errorUi->DisplayMessage("Bu okul no'ya sahip başka bir öğrenci var. Lütfen başka bir okul no kullanın.");
            return false;
        }

        return true;
    }

    bool StudentEditor::checkName(QString& name)
    {
        name = name.trimmed();
        if (name.length() > 40) {
            errorUi->DisplayMessage("Ad veya soyad 40 harften daha uzun olamaz.");
            return false;
        }
        if (name.length() < 1) {
            errorUi->DisplayMessage("Ad veya soyad 1 harften daha kısa olamaz.");
            return false;
        }
        return true;
    }

    // Return true if successful, false if not
    bool StudentEditor::createStudent(int id, QString& firstName, QString& lastName, int grade, QString& section)
    {
        if (checkId(id) && checkName(firstName) && checkName(lastName)) {
            Student* s = new Student { id, firstName, lastName, grade, section };
            db->Add(*s);
            return true;
        }
        return false;
    }

    // Return true if successful, false if not
    bool StudentEditor::updateStudent(Shared::Student& original, int id, QString& firstName, QString& lastName, int grade, QString& section)
    {
        bool isIdValid = original.id == id ? true : checkId(id);
        if (isIdValid && checkName(firstName) && checkName(lastName)) {
            int oldId = original.id;

            original.id = id;
            original.firstName = firstName;
            original.lastName = lastName;
            original.grade = grade;
            original.section = section;

            db->Update(original, oldId);
            return true;
        }
        return false;
    }

} // namespace BLL
} // namespace ikoOSKAR
