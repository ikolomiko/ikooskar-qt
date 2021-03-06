#include "databasehelper.h"
#include <QLocale>
#include <QMessageBox>
#include <algorithm>

namespace ikoOSKAR {
namespace BLL {

    /**
     * @brief Initalizes {@code dal}, {@code databaseCache} and {@code errorUi} properties
     * @param errorUi : A pointer to an instance of ErrorUi with a previously determined title
     * @see dal, databaseCache, errorUi
     */
    DatabaseHelper::DatabaseHelper(UI::ErrorUi* errorUi)
        : dal(new ikoOSKAR::DAL::Database())
        , databaseCache(dal->GetAllStudents())
        , errorUi(errorUi)
    {
    }

    /**
     * @brief Adds the given student to the database and databaseCache
     * @param s : The pointer of the student to be added to the database
     * @see ikoOSKAR::DAL::Database::Add()
     */
    void DatabaseHelper::Add(Student* s)
    {
        QString errorMsg = "BLL.Add() fonksiyonunda bir hata oluştu, öğrenci eklenemedi";

        if (IdExists(s->id)) {
            // Another student with the same id already exists
            errorMsg = QString().number(s->id) + " okul no'suna sahip başka bir öğrenci var. "
                                                 "Bu nedenle yeni öğrenci eklenemedi!";
            errorUi->DisplayMessage(errorMsg);
        } else if (dal->Add(s, &errorMsg)) {
            // Student id is unique and student added to the database successfully
            databaseCache->insert(s->id, *s);
        } else {
            // Student id is unique but an error occurred when trying to add the student to the database
            errorUi->DisplayMessage(errorMsg);
        }
    }

    /**
     * @brief Updates the information of the student with id {@code s->id}.
     * @param s : The pointer of a student object with updated credentials
     * @param oldId : The old id of the student {@code s} if its id has changed. Otherwise same as {@code s->id}
     * @see ikoOSKAR::DAL::Database::Update()
     */
    void DatabaseHelper::Update(Student* s, int oldId)
    {
        QString errorMsg = "BLL.Update() fonksiyonunda bir hata oluştu, öğrenci bilgileri güncellenemedi";

        // If the id of the student has changed and there's already another student with the id newId
        if (oldId != s->id && IdExists(s->id)) {
            errorMsg = QString().number(s->id) + " okul no'suna sahip başka bir öğrenci var. "
                                                 "Öğrenci bilgileri güncellenemedi!";
            errorUi->DisplayMessage(errorMsg);
            return;
        }

        if (oldId == s->id && !IdExists(s->id)) {
            // There's no student with the given id, thus cannot update its properties
            errorMsg = QString().number(s->id) + " okul no'su sistemde kayıtlı değil. "
                                                 "Öğrenci bilgileri güncellenemedi!";
            errorUi->DisplayMessage(errorMsg);
            return;
        }

        Student* old = &(*databaseCache)[oldId];
        Student newStudent = {
            .id = s->id,
            .firstName = s->firstName.isEmpty() ? old->firstName : s->firstName,
            .lastName = s->lastName.isEmpty() ? old->firstName : s->lastName,
            .grade = s->grade == 0 ? old->grade : s->grade,
            .section = s->section.isEmpty() ? old->section : s->section
        };

        if (dal->Update(&newStudent, oldId, &errorMsg)) {
            // The update request is successful
            if (oldId != s->id)
                databaseCache->remove(oldId);
            (*databaseCache)[s->id] = newStudent;
        } else {
            // A student with the given id exists but the update request is not successful
            errorUi->DisplayMessage(errorMsg);
        }
    }

    /**
     * @brief Deletes the student with the given id from the database and databaseCache
     * @param id : The id of the student to be deleted
     */
    void DatabaseHelper::Delete(int id)
    {
        QString errorMsg = "BLL.Delete() fonksiyonunda bir hata oluştu, öğrenci silinemedi";

        if (!IdExists(id)) {
            // There's no student with the given id, thus cannot delete it
            errorMsg = QString().number(id) + " okul no'su sistemde kayıtlı değil. "
                                              "Bu nedenle öğrenci silinemedi!";
            errorUi->DisplayMessage(errorMsg);
        } else if (dal->Delete(id, &errorMsg)) {
            // A student with the given id exists and its deletion is successful
            databaseCache->remove(id);
        } else {
            // A student with the given id exists but its deletion is not successful
            errorUi->DisplayMessage(errorMsg);
        }
    }

    /**
     * @brief Checks the databaseCache if a student with id {@code id} exists
     * @param id : The id of the student to be checked
     * @return {@code true} if the given id exists, {@code false} if not
     */
    bool DatabaseHelper::IdExists(int id)
    {
        return databaseCache->contains(id);
    }

    /**
     * @brief Gets all the ids in the {@code databaseCache}.
     * @return A list of ids in the form of {@code QList<int>}
     */
    QList<int> DatabaseHelper::GetAllIds()
    {
        return databaseCache->keys();
    }

    /**
     * @brief Executes DAL::Database::EndOfTheYear and handles errors if any occurs
     * @see DAL::Database::EndOfTheYear()
     */
    void DatabaseHelper::EndOfTheYear()
    {
        QString errorMsg = "BLL.EndOfTheYear() fonksiyonunda bir hata oluştu, yıl sonu işlemleri yapılamadı";

        if (dal->EndOfTheYear(&errorMsg)) {
            databaseCache = dal->GetAllStudents();
        } else {
            errorUi->DisplayMessage(errorMsg);
        }
    }

    /**
     * @brief Gets all existing class names in ascending order
     *
     * @details All the class names are in <grade>-<section> notation, eg; "9-A" or "12-D"
     * @return A sorted list of all existing class names in <grade>-<section> notation
     */
    QList<QString> DatabaseHelper::GetClassNames()
    {
        auto uniqueClassnames = new QSet<QString>();

        for (auto s : *databaseCache)
            uniqueClassnames->insert(QString().number(s.grade) + "-" + s.section);

        // Sorts the elements in 'sortedClassnames' by their lengths first,
        // and if their lengths are equal, uses the regular string comparison
        auto sortedClassnames = uniqueClassnames->values();
        std::sort(sortedClassnames.begin(), sortedClassnames.end(),
            [](const QString& first, const QString& second) {
                return first.size() != second.size() ? first.size() < second.size() : first < second;
            });

        return sortedClassnames;
    }

    /**
     * @brief Gets all the students that belong to specified grade and section
     * @param grade : The grade to be searched for
     * @param section : The section to be searched for
     * @return A list of pointers to the students that belong to the specified grade and section
     */
    QList<Student*> DatabaseHelper::GetStudentsByClassName(int grade, QString& section)
    {
        QList<Student*> result;
        for (auto s : *databaseCache) {
            if (s.grade == grade && s.section == section) {
                result.append(&s);
            }
        }
        return result;
    }

    /**
     * @brief Same as the original overload, except this one takes the class name as a whole string
     * @param className : The class name to be searched for, in <grade>-<section> notation. (Eg: "9-A" or "11-D")
     * @return A list of pointers to the students that belong to the specified class name
     */
    QList<Student*> DatabaseHelper::GetStudentsByClassName(QString& className)
    {
        int grade = className.length() == 3 ? className.leftRef(1).toInt() : className.leftRef(2).toInt();
        QString section = className.right(1);
        return GetStudentsByClassName(grade, section);
    }

    /**
     * @brief Searches for a student with the given id, displays an error message if such a student not found
     * @param id : The id of the student to be searched for
     * @return A pointer to the student with the given id, or nullptr if the student was not found
     */
    Student* DatabaseHelper::GetStudentById(int id)
    {
        Student* s = nullptr;
        if (databaseCache->contains(id))
            s = &(*databaseCache)[id];
        else
            errorUi->DisplayMessage(QString::number(id) + " okul no'suna sahip öğrenci bulunamadı!");
        return s;
    }

    /**
     * @brief Checks the given parameters if they're suitable for adding/updating. If so, returns a student pointer
     * containing these parameters. Otherwise, displays an error message about what's wrong and returns nullptr.
     * This function is supposed to be used when adding/updating an individual student with manually given parameters.
     * @param id : The id to be checked (the new id if the student is being updated)
     * @param firstName : The first name to be checked
     * @param lastName : The last name to be checked
     * @param grade : The grade to be checked
     * @param section : The section to be checked
     * @return A new student pointer if every parameter is suitable. Otherwise, nullptr
     */
    Student* DatabaseHelper::CheckForManuallyEnteredValues(int id, QString& firstName, QString& lastName, int grade, QString& section)
    {
        bool exists = IdExists(id);
        bool isEverythingOk = true;
        QLocale turkish(QLocale::Turkish);

        auto displayError = [&](QString message) {
            errorUi->DisplayMessage(message);
            isEverythingOk = false;
        };

        auto formatForFirstName = [&](QString& raw) {
            QString temp = raw;
            temp = turkish.toLower(raw.trimmed());
            temp[0] = turkish.toUpper(temp.at(0)).at(0);
            return temp;
        };

        if (firstName.trimmed().isEmpty() || firstName.isNull() || lastName.trimmed().isEmpty() || lastName.isNull())
            displayError("Lütfen ad ve soyadı boş bırakmayınız");
        else if (section.trimmed().isEmpty() || section.isNull())
            displayError("Lütfen bir şube seçiniz");
        else if (exists)
            displayError("Bu okul no'ya sahip başka bir öğrenci var");

        if (!isEverythingOk)
            return nullptr;

        return new Student {
            .id = id,
            .firstName = formatForFirstName(firstName),
            .lastName = turkish.toUpper(lastName.trimmed()),
            .grade = grade,
            .section = section.trimmed().toUpper()
        };
    }

    /**
     * @brief Frees the dynamically allocated memory spaces for dal, databaseCache and errorUi
     * @see dal, databaseCache, errorUi
     */
    DatabaseHelper::~DatabaseHelper()
    {
        delete dal;
        delete databaseCache;
        delete errorUi;
    }

} // namespace BLL
} // namespace ikoOSKAR
