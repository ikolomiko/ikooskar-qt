#ifndef IKOOSKAR_DAL_MULTIIMPORT_H
#define IKOOSKAR_DAL_MULTIIMPORT_H
#include <QList>

namespace ikoOSKAR {
namespace DAL {

    class MultiImport {
    private:
        QString xlsFilePath;
        QStringList trimStringList(const QStringList& list);

    public:
        MultiImport();
        bool importXlsFile(const QString& xlsFilePath);
        QList<QStringList>* getTrimmedLines();
    };

} // namespace DAL
} // namespace ikoOSKAR

#endif // IKOOSKAR_DAL_MULTIIMPORT_H
