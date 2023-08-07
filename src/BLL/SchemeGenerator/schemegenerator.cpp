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

    int SchemeGenerator::deskIndex(int row, int col)
    {
        return (row * 6) + col + 1;
    }

    bool SchemeGenerator::isDeskSuitable(int grade, DeskCoordinates deskCoord)
    {
        Hall* hall = deskCoord.hall;
        int row = deskCoord.row;
        int col = deskCoord.col;

        auto& desk = hall->layout.desks[row][col];
        if (!desk->exists || !desk->isEmpty) {
            return false;
        }
        if (attendingGrades.count() == 1) {
            return true;
        }

        const auto compareToLeft = [&]() {
            if (col == 0) {
                return true;
            }
            auto& leftDesk = hall->layout.desks[row][col - 1];
            if (!leftDesk->exists || leftDesk->isEmpty) {
                return true;
            }
            return leftDesk->student->grade != grade;
        };

        const auto compareToRight = [&]() {
            if (col == 5) {
                return true;
            }
            auto& rightDesk = hall->layout.desks[row][col + 1];
            if (!rightDesk->exists || rightDesk->isEmpty) {
                return true;
            }
            return rightDesk->student->grade != grade;
        };

        return compareToLeft() && compareToRight();
    }

    void SchemeGenerator::placeStudent(const DeskCoordinates& coord, ExamStudent* s)
    {
        auto& [hall, row, col] = coord;
        auto& desk = hall->layout.desks[row][col];
        desk->student = s;
        desk->isEmpty = false;
        s->hallName = hall->name;
        s->deskIndex = deskIndex(row, col);
    }

    Scheme SchemeGenerator::finalizeScheme(Scheme& scheme)
    {
        // Assign the hall layouts
        scheme.hallLayouts.clear();
        for (const auto& hall : std::as_const(*examHalls)) {
            scheme.hallLayouts.insert(hall->name, *hall);
        }

        // Create the class lists
        scheme.classLists.clear();
        for (const auto& className : attendingClassNames) {
            auto [grade, section] = BLL::DatabaseHelper::ParseClassName(className);
            QList<ExamStudent> students;
            for (const auto& s : *studentsByGrade[grade]) {
                if (s->section == section) {
                    students.append(*s);
                }
            }
            scheme.classLists.insert(className, students);
        }

        return scheme;
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
            for (const auto& s : *students) {
                attendingStudents->append(new ExamStudent(*s));
            }

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
        totalCapacity = 0;
        for (const auto& hallName : hallNames) {
            Hall* hall = DatabaseHelper::getInstance()->GetHallByName(hallName);
            examHalls->insert(hallName, hall);
            totalCapacity += hall->capacity;
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
        scheme.name = examName;
        scheme.date = examDate;

        // Count total number of variants across halls
        QHash<Pattern::Variant, int> variantCounts;
        for (const auto& hall : std::as_const(*examHalls)) {
            hall->pattern = Pattern::random(attendingGrades.count());

            for (int i = 0; i < attendingGrades.count(); i++) {
                auto v = (Pattern::Variant)i;
                int count = variantCounts[v] + hall->countVariant(v);
                variantCounts.insert(v, count);
            }
        }

        // Create a list of available variants sorted by their total counts
        QList<Pattern::Variant> sortedVariants = variantCounts.keys();
        std::sort(sortedVariants.begin(), sortedVariants.end(), [&](Pattern::Variant v1, Pattern::Variant v2) {
            return variantCounts[v1] < variantCounts[v2];
        });

        // Create a hashmap of list of students
        /*
            studentsByGrade = {
                9:  [ExamStudent*]* (count: 20)
                10: [ExamStudent*]* (count: 10)
                11: [ExamStudent*]* (count: 30)
            }
        */
        studentsByGrade.clear();
        for (int grade : attendingGrades) {
            studentsByGrade.insert(grade, new QList<ExamStudent*>());
        }
        for (const auto& s : *attendingStudents) {
            studentsByGrade[s->grade]->append(s);
        }

        // Shuffle the lists of students
        auto rng = std::default_random_engine {};
        for (auto* list : std::as_const(studentsByGrade)) {
            std::shuffle(list->begin(), list->end(), rng);
        }

        // Sort the list of grades by their attending population
        std::sort(attendingGrades.begin(), attendingGrades.end(), [&](int g1, int g2) {
            return studentsByGrade[g1]->count() < studentsByGrade[g2]->count();
        });

        // Generate the variant->grade mappings by their count
        /*
            variantToGrade = {
                a: 10
                b: 11
                c: 9
            }
        */
        QHash<Pattern::Variant, int> variantToGrade;
        for (int i = 0; i < attendingGrades.count(); i++) {
            auto variant = sortedVariants[i];
            int grade = attendingGrades[i];
            variantToGrade.insert(variant, grade);
        }

        // First round of student placement: the direct approach
        for (const auto& hall : std::as_const(*examHalls)) {
            for (int row = 0; row < hall->layout.rowCount; row++) {
                for (int col = 0; col < 6; col++) {
                    auto variant = hall->pattern->variantAt(row, col);
                    int grade = variantToGrade[variant];

                    if (!isDeskSuitable(grade, { hall, row, col })) {
                        continue;
                    }
                    if (!studentsByGrade[grade]->empty()) {
                        auto student = studentsByGrade[grade]->takeLast();
                        placeStudent({ hall, row, col }, student);
                    }
                }
            }
        }

        // Second round: find suitable desks for remaining students, disregarding the pattern variant
        QList<ExamStudent*> remainingStudents;
        for (const auto& list : std::as_const(studentsByGrade)) {
            remainingStudents.append(*list);
        }

        QList<ExamStudent*> placedStudents;
        for (const auto& student : remainingStudents) {
            bool placed = false;
            for (const auto& hall : std::as_const(*examHalls)) {
                for (int row = 0; row < hall->layout.rowCount; row++) {
                    for (int col = 0; col < 6; col++) {
                        int grade = student->grade;

                        if (isDeskSuitable(grade, { hall, row, col })) {
                            placeStudent({ hall, row, col }, student);
                            placedStudents.append(student);
                            if (remainingStudents.size() == placedStudents.size()) {
                                // All remaining students have been placed
                                return finalizeScheme(scheme);
                            }
                            placed = true;
                            break;
                        }
                    }
                    if (placed)
                        break;
                }
                if (placed)
                    break;
            }
        }

        // Remove placed students from remaining students
        for (const auto& student : placedStudents) {
            remainingStudents.removeAll(student);
        }
        placedStudents.clear();

        // Third round: for each remaining student s0, for each empty desk d0, for each placed student s1:
        // try to place s1 into d0, and s0 into s1->originalDesk. Also disregarding the pattern variant here.
        // This round might be very resource intensive.
        QList<DeskCoordinates> emptyDesks;
        for (const auto& hall : std::as_const(*examHalls)) {
            for (int row = 0; row < hall->layout.rowCount; row++) {
                for (int col = 0; col < 6; col++) {
                    auto& desk = hall->layout.desks[row][col];
                    if (desk->exists && desk->isEmpty) {
                        emptyDesks.append({ hall, row, col });
                    }
                }
            }
        }

        QList<DeskCoordinates> filledDesks;
        for (const auto& s0 : remainingStudents) {
            bool placed = false;
            for (const auto& d0 : emptyDesks) {
                for (auto& s1 : *attendingStudents) {
                    if (!s1->hallName.isEmpty()) {
                        // ensuring that s1 was placed to a desk
                        continue;
                    }

                    Hall* h1 = examHalls->value(s1->hallName);
                    DeskCoordinates d1 = { h1, s1->deskIndex };
                    if (isDeskSuitable(s1->grade, d0) && isDeskSuitable(s0->grade, d1)) {
                        // Move s1 to d0
                        placeStudent(d0, s1);

                        // Place s0 to d1
                        placeStudent(d1, s0);

                        placedStudents.append(s0);
                        filledDesks.append(d0);
                        if (remainingStudents.size() == placedStudents.size()) {
                            // All remaining students have been placed
                            return finalizeScheme(scheme);
                        }
                        placed = true;
                        break;
                    }
                }
                if (placed)
                    break;
            }
        }
        // Remove placed students from remaining students
        for (const auto& student : placedStudents) {
            remainingStudents.removeAll(student);
        }
        placedStudents.clear();

        // Remove filled desks from empty desks
        for (const auto& desk : filledDesks) {
            emptyDesks.removeAll(desk);
        }
        filledDesks.clear();

        // Fourth round: just place the remaining students to the empty desks with no criteria at all
        // For grades with similar populations, this round should never run
        for (const auto& student : remainingStudents) {
            auto d = emptyDesks.takeLast();
            placeStudent(d, student);
        }

        return finalizeScheme(scheme);
    }

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
