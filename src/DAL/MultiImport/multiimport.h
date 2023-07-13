#ifndef IKOOSKAR_DAL_MULTIIMPORT_H
#define IKOOSKAR_DAL_MULTIIMPORT_H
#include <QList>
#include <QTemporaryDir>

namespace ikoOSKAR {
namespace DAL {

    enum OfficeSuite {
        LibreOffice,
        MsOffice
    };

    class MultiImport {
    private:
        QTemporaryDir* tempDir;
        OfficeSuite officeSuite;
        QString* csvConverterPathWindows(OfficeSuite officeSuite);
        QString* csvConverterPathLinux(OfficeSuite officeSuite);
        bool libreOfficeExists(const QString& path);
        QString* getProgramPathWindows(const QString& programName);
        QStringList* libreOfficeArgs();
        QStringList* msOfficeArgs();
        QStringList csvSplit(const QString& line);
        QStringList trimStringList(const QStringList& list);

    public:
        MultiImport();
        QString* getCsvConverterPath(OfficeSuite officeSuite);
        QStringList* getCsvConverterArgs();
        QString* getCsvPath();
        void importXlsFile(const QString& xlsFilePath);
        QList<QStringList>* getTrimmedCsvLines();
        ~MultiImport();
    };

} // namespace DAL
} // namespace ikoOSKAR

#endif // IKOOSKAR_DAL_MULTIIMPORT_H
