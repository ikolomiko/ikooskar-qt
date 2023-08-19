#include "upgradeassistant.h"
#include "BLL/DatabaseHelper/databasehelper.h"
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

    UpgradeAssistant::UpgradeAssistant() { }

    UpgradeAssistant::~UpgradeAssistant()
    {
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
        if (BLL::DatabaseHelper::getInstance()->GetAllIds().count() > 0) {
            // The current database isn't empty, add a warning to the info message
            text += " Bu işlem, ikoOSKAR'ın yeni sürümündeki öğrenci kayıtlarını silecek ve eski sürümündeki öğrenci kayıtlarını yeni sürüme ekleyecektir.";
        }

        QCheckBox* checkbox = new QCheckBox("Bunu bir daha sorma");
        connect(checkbox, &QCheckBox::stateChanged, [](int state) {
            bool checked = static_cast<Qt::CheckState>(state) == Qt::CheckState::Checked;
            QSettings().setValue("canUpgrade", !checked);
        });
        QMessageBox msgbox(QMessageBox::Icon::Question, title, text);
        msgbox.addButton(QMessageBox::Yes);
        msgbox.addButton(QMessageBox::No);
        msgbox.setCheckBox(checkbox);

        auto val = msgbox.exec();
        qDebug() << (QMessageBox::StandardButtons)val;
        return val == QMessageBox::StandardButton::Yes;
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
    }

} // namespace BLL
} // namespace ikoOSKAR
