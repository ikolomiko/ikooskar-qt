#include "historyprovider.h"

namespace ikoOSKAR {
namespace BLL {

    HistoryProvider::HistoryProvider()
    {
    }

    QMap<QDate, QList<Shared::Scheme>> HistoryProvider::getHistory()
    {
        QMap<QDate, QList<Shared::Scheme>> history;
        for (int i = 1; i < 6; i++) {
            QString s = "01.0" + QString::number(i) + ".2023";
            QDate date = QDate::fromString(s, "dd.MM.yyyy");
            QList<Shared::Scheme> ls;
            for (int j = 0; j < 3; j++) {
                Shared::Scheme s;
                s.name = "Sınav adı " + QString::number(j);
                s.date = date.toString("dd/MM/yyyy");

                ls.append(s);
            }
            if (!ls.empty()) {
                history[date] = ls;
            }
        }
        return history;
    }

    int HistoryProvider::getCurrentTermYear()
    {
        return QDate::fromString("01.01.2023", "dd.MM.yyyy").year();
    }

    int HistoryProvider::getHistoryCount(int termYear)
    {
        (void)termYear;
        return 10;
    }

} // namespace BLL
} // namespace ikoOSKAR
