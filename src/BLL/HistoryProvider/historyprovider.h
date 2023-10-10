#ifndef IKOOSKAR_BLL_HISTORYPROVIDER_H
#define IKOOSKAR_BLL_HISTORYPROVIDER_H

#include "Shared/scheme.h"
#include <QDate>
#include <QList>
#include <QMap>

namespace ikoOSKAR {
namespace BLL {

    class HistoryProvider {
    public:
        HistoryProvider();
        QMap<QDate, QList<Shared::Scheme>> getHistory();
        int getCurrentTermYear();
        int getHistoryCount(int termYear);
    };

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_HISTORYPROVIDER_H
