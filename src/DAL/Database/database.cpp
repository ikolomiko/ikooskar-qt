#include "database.h"

#include <QFile>
#include <QStandardPaths>
#include <QString>
#include <QtSql>

namespace ikoOSKAR {
namespace DAL {
    Database::Database(QString& errorMessage)
    {
        this->databasePath = GetDatabasePath(false);
        this->databaseFilePath = GetDatabasePath(true);
        if (!DatabaseFileExists())
            CreateDatabasePath();

        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(databaseFilePath);
        database.open();
        this->CreateDatabase(errorMessage);
    }

    /**
     * @brief Adds the given student to the database
     * @param s : The pointer of the student to be added to the database
     * @param errorMessage : A QString pointer which holds the error message
     * @return True if the student was added to the database successfully, otherwise false.
     */
    bool Database::Add(Student& s, QString& errorMessage)
    {
        QSqlQuery q;
        q.prepare("INSERT INTO students (id, firstname, lastname, grade, section) "
                  "VALUES (:id, :firstname, :lastname, :grade, :section)");
        q.bindValue(":id", s.id);
        q.bindValue(":firstname", s.firstName);
        q.bindValue(":lastname", s.lastName);
        q.bindValue(":grade", s.grade);
        q.bindValue(":section", s.section);

        if (q.exec())
            return true;

        errorMessage = "DAL.Add() fonksiyonunda bir hata oluştu, öğrenci eklenemedi\n" + q.lastError().text();
        return false;
    }

    bool Database::Add(Hall& h, QString& errorMessage)
    {
        QSqlQuery q;
        q.prepare("INSERT INTO halls (name, capacity, layout) "
                  "VALUES (:name, :capacity, :layout)");
        q.bindValue(":name", h.name);
        q.bindValue(":capacity", h.capacity);
        q.bindValue(":layout", h.layout.toJson());

        if (q.exec())
            return true;

        errorMessage = "DAL.Add() fonksiyonunda bir hata oluştu, derslik eklenemedi\n" + q.lastError().text();
        return false;
    }

    bool Database::Delete(int id, QString& errorMessage)
    {
        QSqlQuery q;
        q.prepare("DELETE FROM students WHERE id = (:id)");
        q.bindValue(":id", id);

        if (q.exec())
            return true;

        errorMessage = "DAL.Delete() fonksiyonunda bir hata oluştu, öğrenci silinemedi\n" + q.lastError().text();
        return false;
    }

    bool Database::Delete(Hall& h, QString& errorMessage)
    {
        QSqlQuery q;
        q.prepare("DELETE FROM halls WHERE name = (:name)");
        q.bindValue(":name", h.name);

        if (q.exec())
            return true;

        errorMessage = "DAL.Delete() fonksiyonunda bir hata oluştu, derslik silinemedi\n" + q.lastError().text();
        return false;
    }

    QHash<int, Student*>* Database::GetAllStudents()
    {
        auto students = new QHash<int, Student*>();
        QSqlQuery q("SELECT * FROM students");
        while (q.next()) {
            Student* s = new Student();
            s->id = q.value(0).toInt();
            s->firstName = q.value(1).toString();
            s->lastName = q.value(2).toString();
            s->grade = q.value(3).toInt();
            s->section = q.value(4).toString();

            students->insert(s->id, s);
        }
        return students;
    }

    QHash<QString, Hall*>* Database::GetAllHalls()
    {
        auto halls = new QHash<QString, Hall*>();
        QSqlQuery q("SELECT * FROM halls");
        while (q.next()) {
            Hall* h = new Hall();
            h->name = q.value(0).toString();
            h->capacity = q.value(1).toInt();
            h->layout = Hall::Layout::fromJson(q.value(2).toJsonObject());

            halls->insert(h->name, h);
        }
        return halls;
    }

    bool Database::Update(Student& s, int oldId, QString& errorMessage)
    {
        QSqlQuery q;
        q.prepare("UPDATE students SET firstname=(:firstname), lastname=(:lastname), "
                  "grade=(:grade), section=(:section), id=(:newId) WHERE id=(:oldId)");
        q.bindValue(":oldId", oldId);
        q.bindValue(":newId", s.id);
        q.bindValue(":firstname", s.firstName);
        q.bindValue(":lastname", s.lastName);
        q.bindValue(":grade", s.grade);
        q.bindValue(":section", s.section);

        if (q.exec())
            return true;

        errorMessage = "DAL.Update() fonksiyonunda bir hata oluştu, öğrenci bilgileri "
                       "güncellenemedi\n"
            + q.lastError().text();

        return false;
    }

    bool Database::Update(Hall& h, const QString& oldName, QString& errorMessage)
    {
        QSqlQuery q;
        q.prepare("UPDATE halls SET name=(:name), capacity=(:capacity), "
                  "layout=(:layout) WHERE name=(:oldName)");
        q.bindValue(":oldName", oldName);
        q.bindValue(":name", h.name);
        q.bindValue(":capacity", h.capacity);
        q.bindValue(":layout", h.layout.toJson());

        if (q.exec())
            return true;

        errorMessage = "DAL.Update() fonksiyonunda bir hata oluştu, derslik bilgileri "
                       "güncellenemedi\n"
            + q.lastError().text();

        return false;
    }

    bool Database::CreateDatabase(QString& errorMessage)
    {
        QString errorBody;

        QSqlQuery q;
        q.prepare("CREATE TABLE IF NOT EXISTS students ("
                  "id integer PRIMARY KEY,"
                  "firstname text,"
                  "lastname text,"
                  "grade integer,"
                  "section text )");
        if (!q.exec()) {
            errorBody = q.lastError().text() + "\n";
        }

        q.prepare("CREATE TABLE IF NOT EXISTS halls ("
                  "name text PRIMARY KEY,"
                  "capacity integer,"
                  "layout text )");
        if (!q.exec()) {
            errorBody += q.lastError().text();
        }

        if (errorBody.isEmpty()) {
            // No errors
            return true;
        }

        errorMessage = "Veri tabanı oluşturulamadı: " + errorBody;
        return false;
    }

    QString Database::GetDatabasePath(bool withFileName)
    {
        QString dirName = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        if (!withFileName)
            return dirName;

        QChar seperator = dirName[0] == '/' ? '/' : '\\';
        dirName.append(seperator);
        dirName.append("database.db");
        return dirName;
    }

    bool Database::EndOfTheYear(QString& errorMessage)
    {
        QString cmds[] = { "DELETE FROM students WHERE grade=12",
            "UPDATE students SET grade=12 WHERE grade=11",
            "UPDATE students SET grade=11 WHERE grade=10",
            "UPDATE students SET grade=10 WHERE grade=9" };
        QString errorNumbers = "";
        QString errors = "";

        for (int i = 0; i < 4; i++) {
            QSqlQuery q;
            q.prepare(cmds[i]);
            if (!q.exec()) {
                errors.append(q.lastError().text());
                errorNumbers.append(QString::number(i) + ",");
            }
        }

        if (errors.length() == 0)
            return true;

        errorMessage = "Yıl sonu işlemleri yapılırken şu numaralı adım(lar)da hata oluştu: "
            + errorNumbers + errors;
        return false;
    }

    bool Database::DatabaseFileExists()
    {
        return QFileInfo::exists(this->databaseFilePath) && QFileInfo(databaseFilePath).isFile();
    }

    void Database::CreateDatabasePath()
    {
        QDir dir(this->databasePath);
        if (!dir.exists())
            dir.mkpath(".");
    }

    Database::~Database()
    {
        QSqlDatabase::database(QSqlDatabase::defaultConnection).close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }

} // namespace DAL
} // namespace ikoOSKAR
