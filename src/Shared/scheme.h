#ifndef IKOOSKAR_SHARED_SCHEME_H
#define IKOOSKAR_SHARED_SCHEME_H

#include "app.h"
#include "examstudent.h"
#include "hall.h"
#include <QDate>
#include <QMap>
#include <QString>

namespace ikoOSKAR {
namespace Shared {

    class Scheme {
    public:
        QString name;
        QDate date;
        QList<QPair<QString, QList<ExamStudent>>> classLists; // [ { className, [ExamStudent] } ]
        QList<Hall> hallLayouts; // [Hall]

        QString path() const
        {
            return rootPath() + date.toString("dd.MM.yyyy") + "/" + name;
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
            return rootPath() + examDate.toString("dd.MM.yyyy") + "/" + examName;
        }

        static QString rootPath()
        {
            return ikoOSKAR::App::PATH_DOCS_ROOT + "/Sınav Düzenleri/";
        }
    };

} // namespace Shared
} // namespace ikoOSKAR

#endif // IKOOSKAR_SHARED_SCHEME_H
