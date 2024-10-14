#include "datamigration.h"
#include "app.h"
#include "deps/libxlsxwriter/third_party/minizip/unzip.h"
#include "deps/libxlsxwriter/third_party/minizip/zip.h"
#include <QDirIterator>
#include <iostream>
#include <vector>

namespace ikoOSKAR {
namespace BLL {

    // Returns true if the import is successful, false if not.
    bool DataMigration::importZip(QString zipPath)
    {
        QString destPath = ikoOSKAR::App::pathDocsRoot;

        // Make a backup first
        QFile(ikoOSKAR::App::pathDocsRoot + "/database.db").copy(ikoOSKAR::App::pathDocsRoot + "/eski-database.db");

        auto bPath = zipPath.toLocal8Bit();
        char* path = bPath.data();
        unzFile uf = unzOpen64(path);
        if (!uf)
            return false;

        do {
            char filename[256];
            unz_file_info64 file_info;
            if (unzGetCurrentFileInfo64(uf, &file_info, filename, sizeof(filename), nullptr, 0, nullptr, 0) != UNZ_OK) {
                unzClose(uf);
                return false;
            }

            QString fullPath = destPath + "/" + filename;

            // Create directories if needed
            QDir(fullPath).mkdir(".");

            if (unzOpenCurrentFile(uf) != UNZ_OK) {
                unzClose(uf);
                return false;
            }

            std::string stdPath = fullPath.toStdString();
            const char* cPath = stdPath.c_str();
            FILE* fp = fopen(cPath, "wb");
            if (fp) {
                char buffer[8192];
                int bytesRead;
                while ((bytesRead = unzReadCurrentFile(uf, buffer, sizeof(buffer))) > 0) {
                    fwrite(buffer, 1, bytesRead, fp);
                }
                fclose(fp);
            }

            unzCloseCurrentFile(uf);
        } while (unzGoToNextFile(uf) == UNZ_OK);

        unzClose(uf);
        return true;
    }

    // Returns the path of exported zip file. Returns empty option on error
    std::optional<QString> DataMigration::exportZip()
    {
        QString sourcePath = ikoOSKAR::App::pathDocsRoot;
        std::vector<std::string> inputFiles;

        QDir sourceDir(sourcePath);
        if (!sourceDir.exists()) {
            std::cerr << "source dir does not exist" << std::endl;
            return {};
        }

        auto bPath = zipFileExportPath.toLocal8Bit();
        char* path = bPath.data();
        zipFile zf = zipOpen64(path, APPEND_STATUS_CREATE);
        if (!zf) {
            std::cout << "could not open zip file uwu" << std::endl;
            return {};
        }

        QDirIterator it(sourcePath, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString filePath = it.next();
            if (it.fileInfo().isFile()) {
                inputFiles.push_back(filePath.toStdString());
            }
        }

        for (const auto& file : inputFiles) {
            zip_fileinfo zi = {};
            auto bRelativePath = sourceDir.relativeFilePath(QString::fromStdString(file)).toLocal8Bit();
            char* relativePath = bRelativePath.data();
            if (zipOpenNewFileInZip(zf, relativePath, &zi, nullptr, 0, nullptr, 0, "This application was made by ikolomiko. If you steal this may Allah curse you to eternal damnation.", Z_DEFLATED, Z_DEFAULT_COMPRESSION) != ZIP_OK) {
                zipClose(zf, nullptr);
                std::cerr << "no open file in zip" << std::endl;
                return {};
            }

            FILE* fp = fopen(file.c_str(), "rb");
            if (fp) {
                char buffer[8192];
                int bytesRead;
                while ((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
                    zipWriteInFileInZip(zf, buffer, bytesRead);
                }
                fclose(fp);
            }

            zipCloseFileInZip(zf);
        }

        zipClose(zf, nullptr);

        return { zipFileExportPath };
    }

    bool DataMigration::isZipFileValid(QString zipPath)
    {
        auto bPath = zipPath.toLocal8Bit();
        char* path = bPath.data();
        unzFile uf = unzOpen64(path);
        if (!uf)
            return false;

        do {
            char filename[1024];
            unz_file_info64 file_info;
            if (unzGetCurrentFileInfo64(uf, &file_info, filename, sizeof(filename), nullptr, 0, nullptr, 0) != UNZ_OK) {
                unzClose(uf);
                return false;
            }

            if (std::string(filename) == "database.db") {
                unzClose(uf);
                return true;
            }
        } while (unzGoToNextFile(uf) == UNZ_OK);

        unzClose(uf);
        return false;
    }

    bool DataMigration::copyZipFileInto(QString newPath)
    {
        std::cout << zipFileExportPath.toStdString() << std::endl;
        if (QFile::exists(newPath)) {
            QFile::remove(newPath);
        }
        return QFile::copy(zipFileExportPath, newPath);
    }
}
}