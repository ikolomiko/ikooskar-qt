#ifndef IKOOSKAR_SHARED_HALL_H
#define IKOOSKAR_SHARED_HALL_H
#include "Shared/pattern.h"
#include "desk.h"
#include <QJsonObject>
#include <QString>
#include <array>
#include <vector>

using DeskRow = std::array<ikoOSKAR::Shared::Desk, 6>;
using DeskLayout = std::vector<DeskRow>;

namespace ikoOSKAR {
namespace Shared {

    class Hall {
    public:
        class Layout {
        private:
            static Layout fromJson(const QJsonObject& json);
            QJsonObject toJson() const;

        public:
            DeskLayout deskRows;

            Layout();
            Layout(int capacity);
            static Layout fromJsonStr(const QString& jsonStr);
            QString toJsonStr() const;
            void clear();
        };

        QString name;
        int capacity;
        Layout layout;
        Pattern* pattern;

        Hall();
        Hall(const QString& name, int capacity);
        Hall(const QString& name, int capacity, const Layout& layout);
        int countVariant(Pattern::Variant v);
        int countStudents(int grade, const QString& section);
    };

} // namespace Shared
} // namespace ikoOSKAR

#endif // IKOOSKAR_SHARED_HALL_H
