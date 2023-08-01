#ifndef IKOOSKAR_SHARED_HALL_H
#define IKOOSKAR_SHARED_HALL_H
#include "desk.h"
#include <QJsonObject>
#include <QString>

namespace ikoOSKAR {
namespace Shared {

    class Hall {
    public:
        class Layout {
        public:
            int rowCount;
            Desk*** desks;
            static Layout fromJson(const QJsonObject& json);
            QJsonObject toJson() const;

            Layout();
            Layout(int capacity);
        };

        QString name;
        int capacity;
        Layout layout;

        Hall();
        Hall(const QString& name, int capacity);
        Hall(const QString& name, int capacity, const Layout& layout);
    };

} // namespace Shared
} // namespace ikoOSKAR

#endif // IKOOSKAR_SHARED_HALL_H
