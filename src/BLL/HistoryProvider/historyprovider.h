#ifndef IKOOSKAR_BLL_HISTORYPROVIDER_H
#define IKOOSKAR_BLL_HISTORYPROVIDER_H

#include "Shared/scheme.h"
#include <QDate>
#include <QList>
#include <QMap>

namespace ikoOSKAR {
namespace BLL {

    class HistoryProvider {
    private:
        QMap<QDate, QList<Shared::Scheme>> history;
        int currentTermStartYear;
        int historyCountForCurrentTerm;

        void populateHistory();

    public:
        HistoryProvider();
        QMap<QDate, QList<Shared::Scheme>> getHistory();
        QString getCurrentTermString();
        int getHistoryCount(int termStartYear);
        int getHistoryCountForCurrentTerm();
        void refresh();
    };

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_HISTORYPROVIDER_H
