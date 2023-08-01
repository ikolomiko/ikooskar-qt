#ifndef IKOOSKAR_SHARED_HALL_H
#define IKOOSKAR_SHARED_HALL_H
#include "desk.h"
#include <QJsonObject>
#include <QString>

namespace ikoOSKAR {
namespace Shared {

    class Hall {
    public:
        QString name;
        int capacity;
        int rowCount;
        Desk*** desks;

        Hall();
        Hall(const QString& name, int capacity);
        static Hall fromJson(const QJsonObject& json);
        QJsonObject toJson() const;
    };

} // namespace Shared
} // namespace ikoOSKAR

#endif // IKOOSKAR_SHARED_HALL_H
