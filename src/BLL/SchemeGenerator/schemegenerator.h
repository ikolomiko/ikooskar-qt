#ifndef IKOOSKAR_BLL_SCHEMEGENERATOR_H
#define IKOOSKAR_BLL_SCHEMEGENERATOR_H

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
        public:
            QString examName;
            QString examDate;
            QList<QString> attendingClassNames;
            QList<QString> examHallNames;
            int nGrades;
            int nStudents;
            int totalCapacity;
            Pattern* pattern;
        };

    private:
        QString examName;
        QString examDate;
        QList<QString> attendingClassNames;
        QList<Student*>* attendingStudents;
        QList<int> attendingGrades;
        QList<Hall*>* examHalls;
        int totalCapacity;

        QString examDir();
        bool schemeExists();

    signals:
        void error(const QString& errorMessage);

    public:
        SchemeGenerator();
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
