#pragma once

#include <QString>
#include <optional>
#include <QDir>

namespace ikoOSKAR {
namespace BLL {
    class DataMigration {
    public:
        bool importZip(QString zipPath);
        std::optional<QString> exportZip();
        bool isZipFileValid(QString zipPath);
        bool copyZipFileInto(QString newPath);

    private:
        const QString zipFileExportPath = QDir::tempPath() + "/ikoOSKAR-veriler.zip";
    };

} // namespace BLL
} // namespace ikoOSKAR
