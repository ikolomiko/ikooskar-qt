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

        QString path()
        {
            return QSettings().value("PATH_DOCS_ROOT").toString() + "/Sınav Düzenleri/" + date + "/" + name;
        }
    };

} // namespace Shared
} // namespace ikoOSKAR

#endif // IKOOSKAR_SHARED_SCHEME_H
