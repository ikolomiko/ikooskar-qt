#include "databasehelper.h"
#include "qapplication.h"
#include <QLocale>
#include <QMessageBox>
#include <QStringView>
#include <cmath>

namespace ikoOSKAR {
namespace BLL {

    /**
     * @brief Initalizes {@code dal} and {@code studentCache} properties
     * @see dal, studentCache
     */
    DatabaseHelper::DatabaseHelper()
    {
        QString errorMessage;
        dal = new ikoOSKAR::DAL::Database(errorMessage);

        if (!errorMessage.isEmpty()) {
            emit error(errorMessage);
            QApplication::quit();
        }

        studentCache = dal->GetAllStudents();
        hallCache = dal->GetAllHalls();

        ResizeAllClassrooms();
    }

    /// TODO write doc
    DatabaseHelper* DatabaseHelper::getInstance()
    {
        if (instance == nullptr) {
            instance = new DatabaseHelper();
        }
        return instance;
    }

    void DatabaseHelper::sortClassnames(QList<QString>* classNames)
    {
        std::sort(classNames->begin(), classNames->end(),
            [](const QString& first, const QString& second) {
                return first.size() != second.size() ? first.size() < second.size() : first < second;
            });
    }

    /**
     * @brief Adds the given student to the database and studentCache
     * @param s : The pointer of the student to be added to the database
     * @see ikoOSKAR::DAL::Database::Add(Student&)
     */
    void DatabaseHelper::Add(Student& s)
    {
        QString errorMsg = "BLL.Add() fonksiyonunda bir hata oluştu, " + QString::number(s.id) + " no'lu öğrenci eklenemedi";
        s.firstName = *formatForFirstName(s.firstName);
        s.lastName = *formatForLastName(s.lastName);

        if (IdExists(s.id)) {
            // Another student with the same id already exists
            errorMsg = QString().number(s.id) + " okul no'suna sahip başka bir öğrenci var. "
                                                "Bu nedenle yeni öğrenci eklenemedi!";
            emit error(errorMsg);
        } else if (dal->Add(s, errorMsg)) {
            // Student id is unique and student added to the database successfully
            studentCache->insert(s.id, &s);
            ResizeClassroom(s.grade, s.section);
        } else {
            // Student id is unique but an error occurred when trying to add the student to the database
            emit error(errorMsg);
        }
    }

    /**
     * @brief Adds the given exam hall to the database and hallCache
     * @param h : The pointer of the exam hall to be added to the database
     * @see ikoOSKAR::DAL::Database::Add(Hall&)
     */
    void DatabaseHelper::Add(Hall& h)
    {
        QString errorMsg = "BLL.Add() fonksiyonunda bir hata oluştu, " + h.name + " adlı derslik eklenemedi";

        if (HallNameExists(h.name)) {
            // Another exam hall with the same name already exists
            errorMsg = h.name + " adına sahip başka bir derslik var. "
                                "Bu nedenle yeni derslik eklenemedi!";
            emit error(errorMsg);
        } else if (dal->Add(h, errorMsg)) {
            // Hall name is unique and the exam hall added to the database successfully
            hallCache->insert(h.name, &h);
        } else {
            // Hall name is unique but an error occurred when trying to add the exam hall to the database
            emit error(errorMsg);
        }
    }

    /**
     * @brief Adds all given students to the database and studentCache
     * @attention This method assumes duplicates were already checked
     * @param students : The list of students to be added to the database
     */
    void DatabaseHelper::AddAll(QList<Student*>& students)
    {
        // Assuming this method was called by MultiImportUi,
        // duplicates were already checked by MultiImportHelper.

        if (students.isEmpty()) {
            return;
        }

        QList<int> problematicIds;
        QString errorLog;
        for (auto s : students) {
            QString errorMsg;
            s->firstName = *formatForFirstName(s->firstName);
            s->lastName = *formatForLastName(s->lastName);

            if (dal->Add(*s, errorMsg)) {
                studentCache->insert(s->id, s);
            } else {
                problematicIds.append(s->id);
                errorLog += errorMsg + "\n";
            }
        }

        // No problems occured
        if (problematicIds.empty()) {
            auto s = *students.at(0);
            ResizeClassroom(s.grade, s.section);
            return;
        }

        QString errorHeader = "okul no'suna sahip öğrenci(ler)de hata oluştu, bu öğrenci(ler) veri tabanına eklenemedi. Hata detayları: \n";
        for (int id : problematicIds) {
            errorHeader = QString::number(id) + " " + errorHeader;
        }
        emit error(errorHeader + errorLog);
    }

    /**
     * @brief Updates the information of the student with id {@code s->id}.
     * @param s : The pointer of a student object with updated credentials
     * @param oldId : The old id of the student {@code s} if its id has changed. Otherwise same as {@code s->id}
     * @see ikoOSKAR::DAL::Database::Update(Student&)
     */
    void DatabaseHelper::Update(Student& s, int oldId)
    {
        QString errorMsg = "BLL.Update() fonksiyonunda bir hata oluştu, öğrenci bilgileri güncellenemedi";

        // If the id of the student has changed and there's already another student with the id newId
        if (oldId != s.id && IdExists(s.id)) {
            errorMsg = QString().number(s.id) + " okul no'suna sahip başka bir öğrenci var. "
                                                "Öğrenci bilgileri güncellenemedi!";
            emit error(errorMsg);
            return;
        }

        if (oldId == s.id && !IdExists(s.id)) {
            // There's no student with the given id, thus cannot update its properties
            errorMsg = QString().number(s.id) + " okul no'su sistemde kayıtlı değil. "
                                                "Öğrenci bilgileri güncellenemedi!";
            emit error(errorMsg);
            return;
        }

        Student* old = studentCache->value(oldId);
        Student* newStudent = new Student {
            .id = s.id,
            .firstName = s.firstName.isEmpty() ? old->firstName : s.firstName,
            .lastName = s.lastName.isEmpty() ? old->firstName : s.lastName,
            .grade = s.grade == 0 ? old->grade : s.grade,
            .section = s.section.isEmpty() ? old->section : s.section
        };
        newStudent->firstName = *formatForFirstName(newStudent->firstName);
        newStudent->lastName = *formatForLastName(newStudent->lastName);

        if (dal->Update(*newStudent, oldId, errorMsg)) {
            // The update request is successful
            if (oldId != s.id)
                studentCache->remove(oldId);
            studentCache->insert(s.id, newStudent);
            ResizeClassroom(newStudent->grade, newStudent->section);
        } else {
            // A student with the given id exists but the update request is not successful
            emit error(errorMsg);
        }
    }

    /**
     * @brief Updates the information of the exam hall with name {@code h->name}.
     * @param h : The pointer of a hall object with updated credentials
     * @param oldName : The old name of the exam hall {@code h} if its name has changed. Otherwise same as {@code h->name}
     * @see ikoOSKAR::DAL::Database::Update(Hall&)
     */
    void DatabaseHelper::Update(Hall& h, const QString& oldName)
    {
        QString errorMsg = "BLL.Update() fonksiyonunda bir hata oluştu, derslik bilgileri güncellenemedi";

        // If the name of the hall has changed and there's already another exam hall with the new name
        if (oldName != h.name && HallNameExists(h.name)) {
            errorMsg = h.name + " adına sahip başka bir derslik var. "
                                "Derslik bilgileri güncellenemedi!";
            emit error(errorMsg);
            return;
        }

        if (oldName == h.name && !HallNameExists(h.name)) {
            // There's no hall with the given name, thus cannot update its properties
            errorMsg = h.name + " derslik adı sistemde kayıtlı değil. "
                                "Derslik bilgileri güncellenemedi!";
            emit error(errorMsg);
            return;
        }

        Hall* newHall = new Hall(h.name, h.capacity, h.layout);

        if (dal->Update(*newHall, oldName, errorMsg)) {
            // The update request is successful
            if (oldName != h.name)
                hallCache->remove(oldName);
            hallCache->insert(h.name, newHall);
        } else {
            // An exam hall with the given name exists but the update request is not successful
            emit error(errorMsg);
        }
    }

    /**
     * @brief Deletes the student with the given id from the database and studentCache
     * @param id : The id of the student to be deleted
     */
    void DatabaseHelper::Delete(int id)
    {
        QString errorMsg = "BLL.Delete() fonksiyonunda bir hata oluştu, öğrenci silinemedi";

        if (!IdExists(id)) {
            // There's no student with the given id, thus cannot delete it
            errorMsg = QString().number(id) + " okul no'su sistemde kayıtlı değil. "
                                              "Bu nedenle öğrenci silinemedi!";
            emit error(errorMsg);
        } else if (dal->Delete(id, errorMsg)) {
            // A student with the given id exists and its deletion is successful
            studentCache->remove(id);
        } else {
            // A student with the given id exists but its deletion is not successful
            emit error(errorMsg);
        }
    }

    /**
     * @brief Deletes the student with the given id from the database and studentCache
     * @param id : The id of the student to be deleted
     */
    void DatabaseHelper::Delete(Hall& hall)
    {
        QString errorMsg = "BLL.Delete() fonksiyonunda bir hata oluştu, derslik silinemedi";

        if (!HallNameExists(hall.name)) {
            // There's no exam hall with the given name, thus cannot delete it
            errorMsg = hall.name + " adlı derslik sistemde kayıtlı değil. "
                                   "Bu nedenle derslik silinemedi!";
            emit error(errorMsg);
        } else if (dal->Delete(hall, errorMsg)) {
            // An exam hall with the given name exists and its deletion is successful
            hallCache->remove(hall.name);
        } else {
            // An exam hall with the given name exists but its deletion is not successful
            emit error(errorMsg);
        }
    }

    /**
     * @brief Checks the studentCache if a student with id {@code id} exists
     * @param id : The id of the student to be checked
     * @return {@code true} if the given id exists, {@code false} if not
     */
    bool DatabaseHelper::IdExists(int id)
    {
        return studentCache->contains(id);
    }

    /**
     * @brief Checks the hallCache if an exam hall with name {@code hallName} exists
     * @param hallName : The name of the exam hall to be checked
     * @return {@code true} if the given hall name exists, {@code false} if not
     */
    bool DatabaseHelper::HallNameExists(const QString& hallName)
    {
        return hallCache->contains(hallName);
    }

    /**
     * @brief Gets all the ids in the {@code studentCache}.
     * @return A list of ids in the form of {@code QList<int>}
     */
    QList<int> DatabaseHelper::GetAllIds()
    {
        return studentCache->keys();
    }

    /**
     * @brief Gets all the hall names in the {@code hallCache}.
     * @return A list of hall names in the form of {@code QList<QString>}
     */
    QList<QString> DatabaseHelper::GetAllHallNames()
    {
        return hallCache->keys();
    }

    QList<Hall*>* DatabaseHelper::GetHallsByName(const QList<QString>& hallNames)
    {
        auto result = new QList<Hall*>();
        for (const auto& name : hallNames) {
            result->append(hallCache->value(name));
        }

        return result;
    }

    /**
     * @brief Executes DAL::Database::EndOfTheYear and handles errors if any occurs
     * @see DAL::Database::EndOfTheYear()
     */
    void DatabaseHelper::EndOfTheYear()
    {
        QString errorMsg = "BLL.EndOfTheYear() fonksiyonunda bir hata oluştu, yıl sonu işlemleri yapılamadı";

        if (dal->EndOfTheYear(errorMsg)) {
            studentCache = dal->GetAllStudents();
        } else {
            emit error(errorMsg);
        }
    }

    /**
     * @brief Gets all existing class names in ascending order
     *
     * @details All the class names are in <grade>-<section> notation, eg; "9-A" or "12-D"
     * @return A sorted list of all existing class names in <grade>-<section> notation
     */
    QList<QString>* DatabaseHelper::GetClassNames()
    {
        auto uniqueClassnames = new QSet<QString>();

        for (auto const& s : *studentCache)
            uniqueClassnames->insert(QString().number(s->grade) + "-" + s->section);

        // Sorts the elements in 'sortedClassnames' by their lengths first,
        // and if their lengths are equal, uses the regular string comparison
        auto* sortedClassnames = new QList(uniqueClassnames->values());
        sortClassnames(sortedClassnames);

        return sortedClassnames;
    }

    /**
     * @brief Gets all the students that belong to specified grade and section
     * @param grade : The grade to be searched for
     * @param section : The section to be searched for
     * @return A list of pointers to the students that belong to the specified grade and section
     */
    QList<Student*>* DatabaseHelper::GetStudentsByClassName(int grade, const QString& section)
    {
        QList<Student*>* result = new QList<Student*>();
        for (const auto& s : *studentCache) {
            if (s->grade == grade && s->section == section) {
                result->append(s);
            }
        }
        return result;
    }

    /**
     * @brief Same as the original overload, except this one takes the class name as a whole string
     * @param className : The class name to be searched for, in <grade>-<section> notation. (Eg: "9-A" or "11-D")
     * @return A list of pointers to the students that belong to the specified class name
     */
    QList<Student*>* DatabaseHelper::GetStudentsByClassName(const QString& className)
    {
        auto pair = ParseClassName(className);
        int grade = pair.first;
        QString section = pair.second;
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
        if (studentCache->contains(id))
            s = studentCache->value(id);
        else
            emit error(QString::number(id) + " okul no'suna sahip öğrenci bulunamadı!");
        return s;
    }

    Hall* DatabaseHelper::GetHallByName(const QString& hallName)
    {
        Hall* h = nullptr;
        if (hallCache->contains(hallName)) {
            h = hallCache->value(hallName);
        } else {
            emit error(hallName + " adlı derslik bulunamadı!");
        }
        return h;
    }

    int DatabaseHelper::GetNumberOfStudents()
    {
        return studentCache->count();
    }

    void DatabaseHelper::DeleteEntireClass(const QString& className)
    {
        auto pair = ParseClassName(className);
        int grade = pair.first;
        QString section = pair.second;
        QList<int> idsToDelete;
        for (const auto& s : *studentCache)
            if (s->grade == grade && s->section == section)
                idsToDelete.append(s->id);

        for (const auto& id : idsToDelete)
            Delete(id);
    }

    QPair<int, QString> DatabaseHelper::ParseClassName(const QString& className)
    {
        auto classNameTokens = className.split('-');
        int grade = classNameTokens.at(0).toInt();
        QString section = classNameTokens.at(1);
        return QPair<int, QString>(grade, section);
    }

    /**
     * @brief Adds @code{n} new desks to the given exam hall
     * @param h : Exam hall
     * @param n : Number of desks to be added
     */
    void DatabaseHelper::AddDesk(Hall& h, int n)
    {
        Hall::Layout layout = h.layout;
        int lastRowIdx = layout.rowCount - 1;
        auto lastRow = layout.desks[lastRowIdx];
        int emptyIdx = 5;
        for (; emptyIdx > 0; emptyIdx--) {
            if (lastRow[emptyIdx] != nullptr) {
                emptyIdx += 1;
                break;
            }
        }

        h.capacity += n;
        int emptyDesks = 6 - emptyIdx;
        int rowsToAdd = (n > emptyDesks) ? std::ceil((float)(n - emptyDesks) / 6) : 0;

        if (rowsToAdd == 0) {
            // Add n desks to the last row
            for (int i = 0; i < n; i++) {
                layout.desks[lastRowIdx][emptyIdx + i] = new Desk();
            }
        } else {
            layout.rowCount += rowsToAdd;
            Desk*** desks = new Desk**[layout.rowCount];

            // Copy the rows from the old layout
            for (int row = 0; row <= lastRowIdx; row++) {
                desks[row] = layout.desks[row];
            }

            // Fill the empty desks at the end of the (old) last row
            for (int col = emptyIdx; col < 6; col++) {
                desks[lastRowIdx][col] = new Desk();
            }

            // Add new rows
            for (int row = lastRowIdx + 1; row < layout.rowCount; row++) {
                desks[row] = new Desk*[6];
            }

            // Fill the new rows
            for (int row = lastRowIdx + 1; row < layout.rowCount - 1; row++) {
                for (int col = 0; col < 6; col++) {
                    desks[row][col] = new Desk();
                }
            }

            // Fill enough desks from the new last row
            int col = 0;
            for (; col < h.capacity % 6; col++) {
                desks[layout.rowCount - 1][col] = new Desk();
            }
            for (; col < 6; col++) {
                desks[layout.rowCount - 1][col] = nullptr;
            }
            layout.desks = desks;
        }
        h.layout = layout;

        Update(h, h.name);
    }

    void DatabaseHelper::ResizeClassroom(int grade, const QString& section)
    {
        // Create new classroom (hall) if no such exam hall exists
        QString hallname = QString::number(grade) + "-" + section;
        int classPopulation = GetStudentsByClassName(grade, section)->size();
        if (!HallNameExists(hallname)) {
            auto hall = new Hall(hallname, classPopulation);
            Add(*hall);
        } else {
            // If such classroom (hall) exists, make sure its capacity >= class population
            // Add new desks if necessary
            Hall* hall = GetHallByName(hallname);
            if (hall->capacity < classPopulation) {
                AddDesk(*hall, classPopulation - hall->capacity);
            }
        }
    }

    void DatabaseHelper::ResizeAllClassrooms()
    {
        for (const auto& c : *GetClassNames()) {
            auto pair = ParseClassName(c);
            ResizeClassroom(pair.first, pair.second);
        }
    }

    QString DatabaseHelper::turkishToUpper(const QString& s)
    {
        QLocale turkish(QLocale::Turkish);
        QString temp(s.trimmed());
        temp.replace(u'i', u'İ');
        return turkish.toUpper(temp);
    }

    QString DatabaseHelper::turkishToLower(const QString& s)
    {
        QLocale turkish(QLocale::Turkish);
        QString temp(s.trimmed());
        temp.replace(u'I', u'ı');
        return turkish.toLower(temp);
    }

    QString* DatabaseHelper::formatForFirstName(const QString& raw)
    {
        QStringList words = turkishToLower(raw).simplified().split(' ');
        for (int i = 0; i < words.size(); i++) {
            words[i][0] = turkishToUpper(words[i][0]).at(0);
        }
        return new QString(words.join(' '));
    }

    QString* DatabaseHelper::formatForLastName(const QString& raw)
    {
        return new QString(turkishToUpper(raw));
    }

    /**
     * @brief Frees the dynamically allocated memory spaces for dal and studentCache
     * @see dal, studentCache
     */
    DatabaseHelper::~DatabaseHelper()
    {
        delete dal;
        delete studentCache;
    }

} // namespace BLL
} // namespace ikoOSKAR
