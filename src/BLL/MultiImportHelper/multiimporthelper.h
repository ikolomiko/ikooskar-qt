#ifndef IKOOSKAR_BLL_MULTIIMPORTHELPER_H
#define IKOOSKAR_BLL_MULTIIMPORTHELPER_H

#include "DAL/MultiImport/multiimport.h"
#include "Shared/student.h"
#include "UI/ErrorUi/errorui.h"
#include <QList>

namespace ikoOSKAR {
namespace BLL {
    using namespace Shared;

    class MultiImportHelper {
    private:
        ikoOSKAR::DAL::MultiImport* dal;

        UI::ErrorUi* errorUi;

        QString* convertToCsv(const QString& xlsFilePath);
        QList<Student*>* parseCsv();
        bool matchesHeaderRow(const QStringList& line);
        bool matchesSectionFooter(const QStringList& line);
        bool matchesFileFooter(const QList<QStringList>& lines);

    public:
        MultiImportHelper(UI::ErrorUi*);
        QList<Student*>* parseXls(const QString& xlsFilePath);
        ~MultiImportHelper();
    };

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_MULTIIMPORTHELPER_H
