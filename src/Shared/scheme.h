#ifndef IKOOSKAR_SHARED_SCHEME_H
#define IKOOSKAR_SHARED_SCHEME_H

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
        QString date;
        QMap<QString, QList<ExamStudent>> classLists; // { className: [ExamStudent] }
        QMap<QString, Hall> hallLayouts; // { hallName: Hall }
    };

} // namespace Shared
} // namespace ikoOSKAR

#endif // IKOOSKAR_SHARED_SCHEME_H
