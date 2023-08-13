#include "multiimport.h"
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>

#ifdef Q_OS_WIN32
#define IS_LINUX (false)
#elif defined(Q_OS_LINUX)
#define IS_LINUX (true)
#else
#error "Platform not supported"
#endif

namespace ikoOSKAR {
namespace DAL {

    MultiImport::MultiImport()
    {
        const QString tempPath = QSettings().value("PATH_DOCS_ROOT").toString() + "/.tmp/";
        auto tempRoot = QDir(tempPath);
        tempRoot.removeRecursively();
        tempRoot.mkpath(".");

        this->tempDir = new QTemporaryDir(tempPath);
    }

    QString* MultiImport::getCsvConverterPath(OfficeSuite officeSuite)
    {
        if (IS_LINUX) {
            // Running on Linux
            return csvConverterPathLinux(officeSuite);
        } else {
            // Running on Windows
            return csvConverterPathWindows(officeSuite);
        }
    }

    QString* MultiImport::getCsvPath()
    {
        return new QString(tempDir->filePath("file.csv"));
    }

    void MultiImport::importXlsFile(const QString& xlsFilePath)
    {
        QFile::copy(xlsFilePath, tempDir->filePath("file.xls"));
    }

    bool MultiImport::libreOfficeExists(const QString& path)
    {
        return QProcess::execute(path, { "--version" }) == 0;
    }

    QString* MultiImport::csvConverterPathLinux(OfficeSuite officeSuite)
    {
        // Return nullptr since MsOffice isn't available for Linux
        if (officeSuite == MsOffice) {
            return nullptr;
        }

        // Check for the LibreOffice binary in the following order:
        // - $PATH/libreoffice (native package manager and snapd packages)
        // - snap package (if somehow /snap/bin isn't in the $PATH)
        // - User-installed Flatpak
        // - Systemwide-installed Flatpak
        QStringList searchPaths = {
            // "libreoffice" works for the first occurence in the $PATH
            // so usually it should cover these cases: /bin/libreoffice, /usr/bin/libreoffice, /usr/local/bin/libreoffice, /var/lib/snapd/snap/bin/libreoffice (if snapd is installed)
            "libreoffice",
            "/snap/bin/libreoffice",
            "/var/lib/snapd/snap/bin/libreoffice",
            QDir::homePath() + "./local/share/flatpak/exports/bin/org.libreoffice/LibreOffice",
            "/var/lib/flatpak/exports/bin/org.libreoffice.LibreOffice",
        };
        for (const auto& path : searchPaths) {
            if (libreOfficeExists(path)) {
                return new QString(path);
            }
        }

        // Return nullptr if there were no matches
        return nullptr;
    }

    QString* MultiImport::getProgramPathWindows(const QString& programName)
    {
        QSettings registry("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\" + programName, QSettings::NativeFormat);
        auto result = registry.value(".").toString();

        if (result.size() == 0) {
            return nullptr;
        } else {
            return new QString(result);
        }
    }

    QString* MultiImport::csvConverterPathWindows(OfficeSuite officeSuite)
    {
        this->officeSuite = officeSuite;
        if (officeSuite == MsOffice) {
            auto excelPath = getProgramPathWindows("EXCEL.EXE");
            if (excelPath == nullptr) {
                // If MS Excel is not installed, return nullptr
                return nullptr;
            } else {
                // If it is found, that means our xls2csv.vbs script can work fine.
                // Create the script and return the name of the script interpreter: wscript.exe
                delete excelPath;
                QFile::copy(":/xls2csv.vbs", tempDir->filePath("xls2csv.vbs"));
                return new QString("wscript.exe");
            }
        } else { // officeSuite == LibreOffice
            // Return the path of the LibreOffice executable (soffice.exe)
            // We will use the soffice.exe directly, without any script.
            return getProgramPathWindows("soffice.exe");
        }
    }

    QStringList* MultiImport::libreOfficeArgs()
    {
        return new QStringList({ "--headless",
            "--convert-to", "csv",
            "--outdir", tempDir->path(),
            tempDir->filePath("file.xls") });
    }

    QStringList* MultiImport::msOfficeArgs()
    {
        return new QStringList({
            tempDir->filePath("xls2csv.vbs"),
            tempDir->filePath("file.xls"),
            tempDir->filePath("file.csv"),
        });
    }

    QStringList* MultiImport::getCsvConverterArgs()
    {
        if (IS_LINUX) {
            // On Linux, only LibreOffice is supported
            return libreOfficeArgs();
        } else {
            // On Windows, both LibreOffice and MsOffice are supported
            if (officeSuite == LibreOffice) {
                return libreOfficeArgs();
            }
            return msOfficeArgs();
        }
    }

    QStringList MultiImport::csvSplit(const QString& line)
    {
        enum State {
            Normal,
            InQuotes
        };
        State state = Normal;
        QStringList tokens;
        QString value;
        bool containsQuotes = false;

        for (int i = 0; i < line.size(); i++) {
            const QChar current = line.at(i);

            if (state == Normal) {
                if (current == ',') {
                    tokens.append(value.trimmed());
                    value.clear();
                } else if (current == '"') {
                    containsQuotes = true;
                    state = InQuotes;
                    value += current;
                } else { // Other char
                    value += current;
                }
            } else { // state == InQuotes
                if (current == '"') {
                    containsQuotes = true;
                    if (i < line.size()) {
                        if (i + 1 < line.size() && line.at(i + 1) == '"') {
                            // A double double quote
                            value += '"';
                            i++;
                        } else {
                            // Ending double quote
                            state = Normal;
                            value += '"';
                        }
                    }
                } else { // Other character
                    value += current;
                }
            }
        }

        if (!value.isEmpty()) {
            tokens.append(value.trimmed());
        }

        if (!containsQuotes) {
            return tokens;
        }

        // Clear quotes and the repetitive whitespace in tokens with quotes
        for (int i = 0; i < tokens.size(); i++) {
            if (tokens[i].length() > 0 && tokens[i].left(1) == '"') {
                tokens[i] = tokens[i].mid(1);
                if (tokens[i].length() > 0 && tokens[i].right(1) == '"')
                    tokens[i] = tokens[i].left(tokens[i].length() - 1);
            }
        }

        return tokens;
    }

    QStringList MultiImport::trimStringList(const QStringList& list)
    {
        QStringList result;

        for (const auto& token : list) {
            const auto simplified = token.simplified();
            if (!simplified.isEmpty()) {
                result.append(simplified);
            }
        }

        return result;
    }

    QList<QStringList>* MultiImport::getTrimmedCsvLines()
    {
        const auto lines = new QList<QStringList>();

        QFile csvFile(*getCsvPath());
        if (csvFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&csvFile);
            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                const auto& tokens = csvSplit(line);
                const auto& trimmed = trimStringList(tokens);
                lines->append(trimmed);
            }
        } else {
            return lines;
        }

        return lines;
    }

    MultiImport::~MultiImport()
    {
        delete tempDir;
    }

} // namespace DAL
} // namespace ikoOSKAR
