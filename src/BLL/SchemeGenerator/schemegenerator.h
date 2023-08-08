#ifndef IKOOSKAR_BLL_SCHEMEGENERATOR_H
#define IKOOSKAR_BLL_SCHEMEGENERATOR_H

#include "Shared/deskcoordinates.h"
#include "Shared/hall.h"
#include "Shared/pattern.h"
#include "Shared/scheme.h"
#include "Shared/student.h"
#include <QDate>
#include <QString>

namespace ikoOSKAR {
namespace BLL {
    using namespace Shared;
    class SchemeGenerator : public QObject {
        Q_OBJECT

    public:
        class Preview {
        private:
            QHash<Pattern::Variant, int> defaultMappings;

        public:
            Preview(QList<int> grades);
            QString examName;
            QString examDate;
            QList<QString> attendingClassNames;
            QList<QString> examHallNames;
            int nGrades;
            int nStudents;
            int totalCapacity;
            Pattern* pattern;
            int gradeAt(int row, int col);
        };

    private:
        QString examName;
        QString examDate;
        QList<QString> attendingClassNames;
        QList<ExamStudent*>* attendingStudents;
        QHash<int, QList<ExamStudent*>*> studentsByGrade;
        QList<int> attendingGrades;
        QHash<QString, Hall*>* examHalls;
        int totalCapacity;

        bool schemeExists();
        int deskIndex(int row, int col);
        bool isDeskSuitable(int grade, DeskCoordinates desk);
        void placeStudent(const DeskCoordinates& coord, ExamStudent* s);
        Scheme finalizeScheme(Scheme& scheme);

    signals:
        void error(const QString& errorMessage);

    public:
        SchemeGenerator();
        ~SchemeGenerator();
        void setDate(const QDate& date);
        bool setName(const QString& name);
        bool setAttendingClasses(const QList<QString>& classNames);
        bool setExamHalls(const QList<QString>& hallNames);
        Preview preview();
        Scheme generate();
    };

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_SCHEMEGENERATOR_H
