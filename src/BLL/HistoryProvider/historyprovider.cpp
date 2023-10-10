#include "historyprovider.h"
#include <QDir>
#define SEPTEMBER (9)

namespace ikoOSKAR {
namespace BLL {

    HistoryProvider::HistoryProvider()
    {
        refresh();
        if (QDate::currentDate().month() < SEPTEMBER) {
            currentTermStartYear = QDate::currentDate().year() - 1;
        } else {
            currentTermStartYear = QDate::currentDate().year();
        }
    }

    void HistoryProvider::populateHistory()
    {
        history.clear();
        QDir rootDir(Shared::Scheme::rootPath());
        rootDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

        QStringList dateStrings = rootDir.entryList();
        for (const auto& dateString : dateStrings) {
            QDate date = QDate::fromString(dateString, "dd.MM.yyyy");
            QDate month(date.year(), date.month(), 1);
            QList<Shared::Scheme>& list = history[month];

            QDir dateDir = rootDir.filePath(dateString);
            dateDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
            QStringList examNames = dateDir.entryList();
            for (const auto& examName : examNames) {
                Shared::Scheme exam;
                exam.name = examName;
                exam.date = date;
                if (QFile::exists(exam.classListPath()) || QFile::exists(exam.hallLayoutPath())) {
                    list.append(exam);
                }
            }

            if (!list.empty()) {
                // Sort in descending order based on dates
                std::sort(list.begin(), list.end(), [](const auto& a, const auto& b) { return a.date > b.date; });
            }
        }
    }

    QMap<QDate, QList<Shared::Scheme>> HistoryProvider::getHistory()
    {
        return history;
    }

    QString HistoryProvider::getCurrentTermString()
    {
        return QString::number(currentTermStartYear) + " - " + QString::number(currentTermStartYear + 1);
    }

    int HistoryProvider::getHistoryCount(int termStartYear)
    {
        // Assuming the academic year begins in september 1st and ends in the next year's july 1st
        QDate begin = QDate::fromString("01.09." + QString::number(termStartYear), "dd.MM.yyyy");
        QDate end = QDate::fromString("01.07." + QString::number(termStartYear + 1), "dd.MM.yyyy");

        int count = 0;
        for (const auto& date : history.keys()) {
            if (date < begin) {
                continue;
            }
            if (date > end) {
                break;
            }

            count += history[date].count();
        }

        return count;
    }

    int HistoryProvider::getHistoryCountForCurrentTerm()
    {
        if (historyCountForCurrentTerm == -1) {
            historyCountForCurrentTerm = getHistoryCount(currentTermStartYear);
        }
        return historyCountForCurrentTerm;
    }

    void HistoryProvider::refresh()
    {
        historyCountForCurrentTerm = -1;
        populateHistory();
    }

} // namespace BLL
} // namespace ikoOSKAR
