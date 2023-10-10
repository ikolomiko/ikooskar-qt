#ifndef IKOOSKAR_SHARED_SCHEME_H
#define IKOOSKAR_SHARED_SCHEME_H

#include "examstudent.h"
#include "hall.h"
#include <QDate>
#include <QMap>
#include <QSettings>
#include <QString>

namespace ikoOSKAR {
namespace Shared {

    class Scheme {
    public:
        QString name;
        QString date;
        QList<QPair<QString, QList<ExamStudent>>> classLists; // [ { className, [ExamStudent] } ]
        QList<Hall> hallLayouts; // [Hall]

        QString path() const
        {
            QString datePath = date;
            datePath.replace("/", ".");
            return QSettings().value("PATH_DOCS_ROOT").toString() + "/Sınav Düzenleri/" + datePath + "/" + name;
        }

        const QString classListPath() const
        {
            return path() + "/Sınıf Karma Listeleri.xlsx";
        }

        const QString hallLayoutPath() const
        {
            return path() + "/Oturma Planları.xlsx";
        }

        static QString path(const QString& examName, const QDate& examDate)
        {
            return QSettings().value("PATH_DOCS_ROOT").toString() + "/Sınav Düzenleri/" + examDate.toString("dd.MM.yyyy") + "/" + examName;
        }
    };

} // namespace Shared
} // namespace ikoOSKAR

#endif // IKOOSKAR_SHARED_SCHEME_H
