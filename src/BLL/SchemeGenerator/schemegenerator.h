#ifndef IKOOSKAR_BLL_SCHEMEGENERATOR_H
#define IKOOSKAR_BLL_SCHEMEGENERATOR_H

#include <QDate>
#include <QString>

namespace ikoOSKAR {
namespace BLL {

    class SchemeGenerator : public QObject {
        Q_OBJECT

    private:
        QString examName;
        QDate examDate;

        QString examDir();
        bool schemeExists();

    signals:
        void error(const QString& errorMessage);

    public:
        SchemeGenerator();
        void setDate(const QDate& date);
        bool setName(const QString& name);
        bool setAttendingClasses(const QList<QString>& classNames);
        // bool setExamHalls(const QList<Shared::Hall>& halls);
        // Shared::Scheme generate();
    };

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_SCHEMEGENERATOR_H
