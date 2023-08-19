#include "upgradeassistant.h"
#include "BLL/DatabaseHelper/databasehelper.h"
#include "DAL/Database/database.h"
#include <QCheckBox>
#include <QDirIterator>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QStandardPaths>

namespace ikoOSKAR {
namespace BLL {

    void UpgradeAssistant::findOldDatabaseFile()
    {
        QString dir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/ikoSoft";
        QDirIterator it(dir, { "ogrenciler.sdf" }, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            auto path = it.next();
            oldDbPath = path;
            break;
        }
    }

    QByteArray UpgradeAssistant::convertToZippedSqlite()
    {
        QNetworkAccessManager manager;
        QString boundary = "boundaryString";

        QNetworkRequest request;
        request.setUrl(QUrl("https://www.rebasedata.com/api/v1/convert?outputFormat=sqlite"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + boundary);

        QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        QFile oldDb(oldDbPath);
        oldDb.open(QFile::ReadWrite);

        QHttpPart filePart;
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"files[]\"; filename=\"ogrenciler.sdf\"");
        filePart.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
        filePart.setBodyDevice(&oldDb);
        multiPart->setBoundary(boundary.toUtf8());
        multiPart->append(filePart);

        QNetworkReply* reply = manager.post(request, multiPart);
        multiPart->setParent(reply);

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() == QNetworkReply::NoError
            && reply->header(QNetworkRequest::ContentTypeHeader).toString() == "application/zip") {
            QByteArray responseData = reply->readAll();
            return responseData;
        } else {
            emit error("Dosya dönüştürme hatası: " + reply->errorString());
        }

        reply->deleteLater();
        return QByteArray();
    }

    QByteArray UpgradeAssistant::unzip(const QByteArray& zipBytes)
    {
        // Unzip
        return zipBytes;
    }

    QString UpgradeAssistant::saveTemp(const QByteArray& unzippedBytes)
    {
        const QString tempPath = QSettings().value("PATH_DOCS_ROOT").toString() + "/.tmp/";
        auto tempRoot = QDir(tempPath);
        tempRoot.removeRecursively();
        tempRoot.mkpath(".");
        tempDir = new QTemporaryDir(tempPath);

        QFile out(tempDir->filePath("database.db"));
        out.open(QIODevice::WriteOnly);
        out.write(unzippedBytes);
        return out.fileName();
    }

    int UpgradeAssistant::modifyDatabase(const QString& path)
    {
        /*
         * Run the following sql commands
            CREATE TABLE "students" (
                "id"	INTEGER,
                "firstname"	TEXT,
                "lastname"	TEXT,
                "grade"	INTEGER,
                "section"	TEXT,
                PRIMARY KEY("id")
            );
            INSERT INTO "main"."students" ("id","firstname","lastname","grade","section") SELECT "okulno","ad","soyad","sinif","sube" FROM "main"."ogrenciler";
            DROP TABLE "main"."ogrenciler";
            SELECT COUNT(id) FROM students;
        */
        // Return the result of the last command
        qDebug() << path;
        return -1;
    }

    UpgradeAssistant::UpgradeAssistant() { }

    UpgradeAssistant::~UpgradeAssistant()
    {
        delete tempDir;
    }

    bool UpgradeAssistant::willUpgrade()
    {
#ifndef Q_OS_WIN32
        // Since the previous versions of ikoOSKAR were Windows-only,
        // there's no need to check for old databases on other platforms.
        return false;
#endif

        // Check if the upgrade assistant was run successfully before
        if (QSettings().value("canUpgrade", true).toBool() == false) {
            return false;
        }

        // Check if the old database file exists
        findOldDatabaseFile();
        if (oldDbPath.isEmpty()) {
            return false;
        }

        // Ask the user before proceeding
        QString title = "ikoOSKAR Güncelleme Yardımcısı";
        QString text = "ikoOSKAR'ın daha eski bir sürümü tespit edildi. Eski sürümdeki öğrenci kayıtları yeni sürüme aktarılsın mı?";
        auto db = BLL::DatabaseHelper::getInstance();
        if (db->GetAllIds().count() > 0) {
            // The current database isn't empty, add a warning to the info message
            text += " Bu işlem, ikoOSKAR'ın yeni sürümündeki öğrenci kayıtlarını silecek ve eski sürümündeki öğrenci kayıtlarını yeni sürüme ekleyecektir.";
        }
        delete db;

        QCheckBox* checkbox = new QCheckBox("Bunu bir daha sorma");
        connect(checkbox, &QCheckBox::stateChanged, [](int state) {
            bool checked = static_cast<Qt::CheckState>(state) == Qt::CheckState::Checked;
            QSettings().setValue("canUpgrade", !checked);
        });
        QMessageBox msgbox(QMessageBox::Icon::Question, title, text);
        msgbox.addButton(QMessageBox::Yes);
        msgbox.addButton(QMessageBox::No);
        msgbox.setCheckBox(checkbox);

        return msgbox.exec() == QMessageBox::Yes;
    }

    void UpgradeAssistant::run()
    {
        // Send http request to the converter api and get the response
        QByteArray zippedSqliteBytes = convertToZippedSqlite();
        if (zippedSqliteBytes.isEmpty() || zippedSqliteBytes.size() < 50) {
            // necessary error message was shown
            emit upgradeFinished(-1);
            return;
        }

        // Unzip the response file
        QByteArray unzippedBytes = unzip(zippedSqliteBytes);
        if (unzippedBytes.isEmpty()) {
            // the response file wasn't a zip file or something went wrong unzipping the response file
            emit upgradeFinished(-2);
            return;
        }

        // Save the new converted database file to a temporary location
        QString tempDbPath = saveTemp(unzippedBytes);

        // Modify the structure of the database and the tables to match the new structure
        int studentsImported = modifyDatabase(tempDbPath);

        if (studentsImported < 0) {
            // Some error occurred during the import process
            emit upgradeFinished(3);
            return;
        }

        // No errors, copy the new db to the default location
        QDir dbDir(DAL::Database::GetDatabasePath());
        if (!dbDir.exists()) {
            dbDir.mkpath(".");
        }

        QFile destination(dbDir.filePath("database.db"));
        if (destination.exists()) {
            destination.remove();
        }
        QFile(tempDbPath).rename(destination.fileName());

        // Database migration successful, don't ask the user for another one
        QSettings().setValue("canUpgrade", false);

        emit upgradeFinished(studentsImported);
    }

} // namespace BLL
} // namespace ikoOSKAR
