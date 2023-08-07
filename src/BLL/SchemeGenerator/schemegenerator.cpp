#include "schemegenerator.h"
#include "BLL/DatabaseHelper/databasehelper.h"
#include <QDir>
#include <QSet>
#include <QSettings>

namespace ikoOSKAR {
namespace BLL {

    SchemeGenerator::SchemeGenerator()
    {
        attendingStudents = new QList<ExamStudent*>();
        examHalls = new QHash<QString, Hall*>();
    }

    SchemeGenerator::~SchemeGenerator()
    {
        for (const auto& hall : std::as_const(*examHalls)) {
            hall->layout.clear();
        }
        delete attendingStudents;
        delete examHalls;
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

        attendingClassNames = classNames;
        attendingGrades.clear();
        attendingStudents->clear();
        for (const auto& className : classNames) {
            // Get all students of selected classes
            auto students = DatabaseHelper::getInstance()->GetStudentsByClassName(className);
            attendingStudents->append(*students);

            int grade = DatabaseHelper::ParseClassName(className).first;
            if (!attendingGrades.contains(grade)) {
                attendingGrades << grade;
            }
        }
        std::sort(attendingGrades.begin(), attendingGrades.end());

        return true;
    }

    bool SchemeGenerator::setExamHalls(const QList<QString>& hallNames)
    {
        examHalls->clear();
        examHalls->append(*DatabaseHelper::getInstance()->GetHallsByName(hallNames));

        totalCapacity = 0;
        for (const auto& h : *examHalls) {
            totalCapacity += h->capacity;
        }

        if (totalCapacity < attendingStudents->size()) {
            emit error("Sınava katılacak öğrenci sayısı, dersliklerdeki sıra sayısından daha fazla! Lütfen derslik sayısını ya da kapasitesini artırınız!");
            return false;
        }

        return true;
    }

    SchemeGenerator::Preview SchemeGenerator::preview()
    {
        QList<QString> examHallNames;
        for (const auto& hall : std::as_const(*examHalls)) {
            examHallNames << hall->name;
        };
        BLL::DatabaseHelper::sortClassnames(&examHallNames);

        Preview p(attendingGrades);
        p.examName = examName;
        p.examDate = examDate;
        p.attendingClassNames = attendingClassNames;
        p.examHallNames = examHallNames;
        p.nGrades = attendingGrades.size();
        p.nStudents = attendingStudents->size();
        p.totalCapacity = totalCapacity;
        p.pattern = new Pattern(attendingGrades.count(), 0);

        return p;
    }

    Scheme SchemeGenerator::generate()
    {
        Scheme scheme;

    SchemeGenerator::Preview::Preview(QList<int> grades)
    {
        std::sort(grades.begin(), grades.end());
        for (int i = 0; i < grades.size(); i++) {
            int grade = grades[i];
            auto v = (Pattern::Variant)i;
            defaultMappings.insert(v, grade);
        }
    }

    int SchemeGenerator::Preview::gradeAt(int row, int col)
    {
        auto v = pattern->variantAt(row, col);
        return defaultMappings[v];
    }

} // namespace BLL
} // namespace ikoOSKAR
