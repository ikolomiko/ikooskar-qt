#include "database.h"
#include <QFile>
#include <QString>
#include <QStandardPaths>
#include <QtSql>
#include <QDebug>

namespace ikoOSKAR { namespace DAL {
    Database::Database(){
        this->databasePath = GetDatabasePath(false);
        this->databaseFilePath = GetDatabasePath(true);

        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(databaseFilePath);
        database.open();
        this->CreateDatabase();
    }

    bool Database::Add(Student const *o){
        QSqlQuery q;
        q.prepare("INSERT INTO students (number, firstname, lastname, grade, section) "
                  "VALUES (:number, :firstname, :lastname, :grade, :section)");
        q.bindValue(":number", o->number);
        q.bindValue(":firstname", o->firstName);
        q.bindValue(":lastname", o->lastName);
        q.bindValue(":grade", o->grade);
        q.bindValue(":section", o->section);

        if(q.exec())
            return true;

        qDebug() << "Error adding new student "
                     << q.lastError();
        return false;

    }

    bool Database::Delete(Student *o){
        QSqlQuery q;
        q.prepare("DELETE FROM students WHERE id = (:id)");
        q.bindValue(":id", o->id);

        if(q.exec())
            return true;

        qDebug() << "Error deleting student "
                 << q.lastError();
        return false;
    }

    QList<Student> *Database::GetAllStudents(){
        QList<Student>* students = new QList<Student>();
        QSqlQuery q("SELECT * FROM students");
        while (q.next()) {
            Student *o = new Student();
            o->id = q.value(0).toInt();
            o->firstName = q.value(1).toString();
            o->lastName = q.value(2).toString();
            o->number = q.value(3).toInt();
            o->grade = q.value(4).toInt();
            o->section = q.value(5).toString();

            students->append(*o);
        }
        return students;
    }

    bool Database::Update(Student *o){
        QSqlQuery q;
        q.prepare("UPDATE students SET number=(:number), firstname=(:firstname), lastname=(:lastname), grade=(:grade), section=(:section) WHERE id=(:id)");
        q.bindValue(":id", o->id);
        q.bindValue(":number", o->number);
        q.bindValue(":firstname", o->firstName);
        q.bindValue(":lastname", o->lastName);
        q.bindValue(":grade", o->grade);
        q.bindValue(":section", o->section);

        if(q.exec())
            return true;

        qDebug() << "Error updating student "
                 << q.lastError();
        return false;
    }

    bool Database::CreateDatabase(){
        QSqlQuery q;
        q.prepare("CREATE TABLE IF NOT EXISTS students ("
                  "id integer PRIMARY KEY AUTOINCREMENT,"
                  "firstname text,"
                  "lastname text,"
                  "number integer NOT NULL,"
                  "grade integer,"
                  "section text )");
        if (q.exec())
            return true;

        qDebug() << "Error creating new database"
                 << q.lastError();
        return false;
    }

    QString Database::GetDatabasePath(bool withFileName){
        QString dirName = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        if (!withFileName) return dirName;

        QChar seperator = dirName[0] == '/' ? '/' : '\\';
        dirName.append(seperator);
        dirName.append("students.db");
        return dirName;
    }

    bool Database::EndOfTheYear() {
        QString cmds[] = {
            "DELETE FROM students WHERE grade=12",
            "UPDATE students SET grade=12 WHERE grade=11",
            "UPDATE students SET grade=11 WHERE grade=10",
            "UPDATE students SET grade=10 WHERE grade=9"
        };
        QString errorNumbers = "";
        QString errors = "";

        for (int i = 0; i < 4; i++){
            QSqlQuery q;
            q.prepare(cmds[i]);
            if (!q.exec()) {
                errors.append(q.lastError().text());
                errorNumbers.append(QString::number(i));
            }
        }

        if (errors.length() == 0) return true;

        qDebug() << "Error doing end of the year operations at operation number(s)" << errorNumbers
                 << errors;
        return false;
    }

    Database::~Database(){
        QSqlDatabase::database(QSqlDatabase::defaultConnection).close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        qDebug() << "this instance of db has destroyed";
    }

}  // namespace DAL
} // namespace ikoOSKAR
