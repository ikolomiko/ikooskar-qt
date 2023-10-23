#ifndef IKOOSKAR_BLL_HISTORYPROVIDER_H
#define IKOOSKAR_BLL_HISTORYPROVIDER_H

#include "Shared/scheme.h"
#include <QDate>
#include <QList>
#include <QMap>

using History = QMap<QDate, QList<ikoOSKAR::Shared::Scheme>>;

namespace ikoOSKAR {
namespace BLL {
    class HistoryProvider : public QObject {
        Q_OBJECT
    private:
        History history;
        int currentTermStartYear;
        int historyCountForCurrentTerm;

        void populateHistory();

    public:
        HistoryProvider();
        QString getCurrentTermString();
        int getHistoryCount(int termStartYear);
        int getHistoryCountForCurrentTerm();
        void refresh();

    signals:
        void historyReady(const History& history);
    };

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_HISTORYPROVIDER_H
