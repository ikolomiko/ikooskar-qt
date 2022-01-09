#include "databasehelper.h"
#include "UI/ErrorUi/errorui.h"
#include <QMessageBox>

namespace ikoOSKAR {
namespace BLL {

    DatabaseHelper::DatabaseHelper(std::function<void(const QString& message)> errHandler)
    {
        dal = new ikoOSKAR::DAL::Database();
        databaseCache = dal->GetAllStudents();
        handleError = errHandler;
    }

    void DatabaseHelper::Add(Student* s)
    {
        QString errorMsg = "BLL.Add() fonksiyonunda bir hata oluştu, öğrenci eklenemedi";

        if (IdExists(s->id)) {
            // Another student with the same id already exists
            errorMsg = QString().number(s->id) + " okul no'suna sahip başka bir öğrenci var. "
                                                 "Bu nedenle yeni öğrenci eklenemedi!";
            handleError(errorMsg);
        } else if (dal->Add(s, &errorMsg)) {
            // Student id is unique and student added to the database successfully
            databaseCache->insert(s->id, *s);
        } else {
            // Student id is unique but an error occurred when trying to add the student to the database
            handleError(errorMsg);
        }
    }

    void DatabaseHelper::Update(Student* s)
    {
        QString errorMsg = "BLL.Update() fonksiyonunda bir hata oluştu, öğrenci bilgileri güncellenemedi";

        if (!IdExists(s->id)) {
            // There's no student with the given id, thus cannot update its properties
            errorMsg = QString().number(s->id) + " okul no'su sistemde kayıtlı değil. "
                                                 "Öğrenci bilgileri güncellenemedi!";
            handleError(errorMsg);
            return;
        }

        Student* old = &(*databaseCache)[s->id];
        Student newStudent = {
            .id = s->id,
            .firstName = s->firstName.isEmpty() ? old->firstName : s->firstName,
            .lastName = s->lastName.isEmpty() ? old->firstName : s->lastName,
            .grade = s->grade == 0 ? old->grade : s->grade,
            .section = s->section.isEmpty() ? old->section : s->section
        };

        if (dal->Update(&newStudent, &errorMsg)) {
            // A student with the given id exists and the update request is successful
            (*databaseCache)[s->id] = newStudent;
        } else {
            // A student with the given id exists but the update request is not successful
            handleError(errorMsg);
        }
    }

    void DatabaseHelper::Delete(Student* s)
    {
        QString errorMsg = "BLL.Delete() fonksiyonunda bir hata oluştu, öğrenci silinemedi";

        if (!IdExists(s->id)) {
            // There's no student with the given id, thus cannot delete it
            errorMsg = QString().number(s->id) + " okul no'su sistemde kayıtlı değil. "
                                                 "Bu nedenle öğrenci silinemedi!";
            handleError(errorMsg);
        } else if (dal->Delete(s, &errorMsg)) {
            // A student with the given id exists and its deletion is successful
            databaseCache->remove(s->id);
            qDebug() << databaseCache->keys();
        } else {
            // A student with the given id exists but its deletion is not successful
            handleError(errorMsg);
        }
    }

    bool DatabaseHelper::IdExists(int id)
    {
        return databaseCache->contains(id);
    }

    QList<int> DatabaseHelper::GetAllIds()
    {
        return databaseCache->keys();
    }

    void DatabaseHelper::EndOfTheYear()
    {
    }

    QList<QString> DatabaseHelper::GetClassNames()
    {
    }

    QList<Student*> DatabaseHelper::GetStudentsByClassName(int grade, QString section)
    {
    }

    Student* DatabaseHelper::GetStudentById(int id)
    {
    }

    bool DatabaseHelper::CheckForPrerequsities()
    {
        return false;
    }

    DatabaseHelper::~DatabaseHelper()
    {
        delete dal;
        delete databaseCache;
    }

} // namespace BLL
} // namespace ikoOSKAR
