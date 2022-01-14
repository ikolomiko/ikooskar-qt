#include "database.h"

#include <QFile>
#include <QStandardPaths>
#include <QString>
#include <QtSql>

namespace ikoOSKAR {
namespace DAL {
    Database::Database()
    {
        this->databasePath = GetDatabasePath(false);
        this->databaseFilePath = GetDatabasePath(true);
        if (!DatabaseFileExists())
            CreateDatabasePath();

        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(databaseFilePath);
        database.open();
        this->CreateDatabase();
    }

    /**
     * @brief Adds the given student to the database
     * @param s : The pointer of the student to be added to the database
     * @param errorMessage : A QString pointer which holds the error message
     * @return True if the student was added to the database successfully, otherwise false.
     */
    bool Database::Add(Student* s, QString* errorMessage)
    {
        QSqlQuery q;
        q.prepare("INSERT INTO students (id, firstname, lastname, grade, section) "
                  "VALUES (:id, :firstname, :lastname, :grade, :section)");
        q.bindValue(":id", s->id);
        q.bindValue(":firstname", s->firstName);
        q.bindValue(":lastname", s->lastName);
        q.bindValue(":grade", s->grade);
        q.bindValue(":section", s->section);

        if (q.exec())
            return true;

        *errorMessage = "DAL.Add() fonksiyonunda bir hata oluştu, öğrenci eklenemedi\n" + q.lastError().text();
        return false;
    }

    bool Database::Delete(int id, QString* errorMessage)
    {
        QSqlQuery q;
        q.prepare("DELETE FROM students WHERE id = (:id)");
        q.bindValue(":id", id);

        if (q.exec())
            return true;

        *errorMessage = "DAL.Delete() fonksiyonunda bir hata oluştu, öğrenci silinemedi\n" + q.lastError().text();
        return false;
    }

    QHash<int, Student>* Database::GetAllStudents()
    {
        auto* students = new QHash<int, Student>();
        QSqlQuery q("SELECT * FROM students");
        while (q.next()) {
            Student* s = new Student();
            s->id = q.value(0).toInt();
            s->firstName = q.value(1).toString();
            s->lastName = q.value(2).toString();
            s->grade = q.value(3).toInt();
            s->section = q.value(4).toString();

            students->insert(s->id, *s);
        }
        return students;
    }

    bool Database::Update(Student* s, int oldId, QString* errorMessage)
    {
        QSqlQuery q;
        q.prepare("UPDATE students SET firstname=(:firstname), lastname=(:lastname), "
                  "grade=(:grade), section=(:section), id=(:newId) WHERE id=(:oldId)");
        q.bindValue(":oldId", oldId);
        q.bindValue(":newId", s->id);
        q.bindValue(":firstname", s->firstName);
        q.bindValue(":lastname", s->lastName);
        q.bindValue(":grade", s->grade);
        q.bindValue(":section", s->section);

        if (q.exec())
            return true;

        *errorMessage = "DAL.Update() fonksiyonunda bir hata oluştu, öğrenci bilgileri "
                        "güncellenemedi\n"
            + q.lastError().text();

        return false;
    }

    bool Database::CreateDatabase()
    {
        QSqlQuery q;
        q.prepare("CREATE TABLE IF NOT EXISTS students ("
                  "id integer PRIMARY KEY,"
                  "firstname text,"
                  "lastname text,"
                  "grade integer,"
                  "section text )");
        if (q.exec())
            return true;

        qDebug() << "Error creating new database" << q.lastError();
        return false;
    }

    QString Database::GetDatabasePath(bool withFileName)
    {
        QString dirName = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        if (!withFileName)
            return dirName;

        QChar seperator = dirName[0] == '/' ? '/' : '\\';
        dirName.append(seperator);
        dirName.append("students.db");
        return dirName;
    }

    bool Database::EndOfTheYear(QString* errorMessage)
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
                errorNumbers.append(QString::number(i));
            }
        }

        if (errors.length() == 0)
            return true;

        *errorMessage = "Yıl sonu işlemleri yapılırken şu numaralı adım(lar)da hata oluştu: "
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
