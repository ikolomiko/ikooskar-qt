#include "schemegenerator.h"
#include "BLL/DatabaseHelper/databasehelper.h"
#include <QDir>
#include <QSet>
#include <QSettings>

namespace ikoOSKAR {
namespace BLL {

    SchemeGenerator::SchemeGenerator()
    {
        attendingStudents = new QList<Student*>();
        examHalls = new QList<Hall*>();
    }

    QString SchemeGenerator::examDir()
    {
        return QSettings().value("PATH_DOCS_ROOT").toString() + "/Sınav Düzenleri/" + examDate + "/" + examName;
    }

    bool SchemeGenerator::schemeExists()
    {
        return QDir(examDir()).exists();
    }

    void SchemeGenerator::setDate(const QDate& date)
    {
        examDate = date.toString("dd.MM.yyyy");
    }

    bool SchemeGenerator::setName(const QString& name)
    {
        if (name.isEmpty()) {
            emit error("Sınav adı boş bırakılamaz!");
            return false;
        }

        examName = name;
        if (schemeExists()) {
            emit error("Seçtiğiniz tarihte ve seçtiğiniz adda bir sınav düzeni zaten oluşturulmuş. Lütfen başka bir tarih ya da başka bir sınav adı seçiniz!");
            return false;
        }

        return true;
    }

    bool SchemeGenerator::setAttendingClasses(const QList<QString>& classNames)
    {
        if (classNames.isEmpty()) {
            emit error("En az bir sınıf seçiniz!");
            return false;
        }

        attendingStudents->clear();
        for (const auto& className : classNames) {
            auto students = DatabaseHelper::getInstance()->GetStudentsByClassName(className);
            attendingStudents->append(*students);
        }
        return true;
    }

    bool SchemeGenerator::setExamHalls(const QList<QString>& hallNames)
    {
        examHalls->clear();
        examHalls->append(*DatabaseHelper::getInstance()->GetHallsByName(hallNames));
        int totalCapacity = 0;
        for (const auto& h : *examHalls) {
            totalCapacity += h->capacity;
        }

        if (totalCapacity < attendingStudents->size()) {
            emit error("Sınava katılacak öğrenci sayısı, dersliklerdeki sıra sayısından daha fazla! Lütfen derslik sayısını ya da kapasitesini artırınız!");
            return false;
        }

        return true;
    }

} // namespace BLL
} // namespace ikoOSKAR
