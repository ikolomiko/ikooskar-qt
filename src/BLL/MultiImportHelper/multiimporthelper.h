#ifndef IKOOSKAR_BLL_MULTIIMPORTHELPER_H
#define IKOOSKAR_BLL_MULTIIMPORTHELPER_H

#include "DAL/MultiImport/multiimport.h"
#include "Shared/student.h"
#include <QList>
#include <QRunnable>

namespace ikoOSKAR {
namespace BLL {
    using namespace Shared;

    class MultiImportHelper : public QObject, public QRunnable {
        Q_OBJECT
    private:
        ikoOSKAR::DAL::MultiImport* dal;

        QString xlsFilePath;

        QString* convertToCsv();
        QList<Student*>* parseCsv();
        QList<Student*>* parseXls();

        bool matchesHeaderRow(const QStringList& line);
        bool matchesSectionFooter(const QStringList& line);
        bool matchesFileFooter(const QList<QStringList>& lines);

    public:
        MultiImportHelper(const QString& xlsFilePath);
        void run() override;
        ~MultiImportHelper();

    signals:
        void parsingFinished(QList<Student*>* result);
        void error(const QString& errorMessage);
    };

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_MULTIIMPORTHELPER_H
