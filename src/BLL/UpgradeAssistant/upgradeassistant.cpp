#include "upgradeassistant.h"
#include "BLL/DatabaseHelper/databasehelper.h"
#include <QCheckBox>
#include <QDirIterator>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>

#include <unistd.h>

namespace ikoOSKAR {
namespace BLL {

    void UpgradeAssistant::findOldDatabaseFile()
    {
        QString dir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/ikoSoft";
        QDirIterator it(dir, { "ogrenciler.sdf" }, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            auto path = it.next();
            oldDbPath = path;
        }
    }

    UpgradeAssistant::UpgradeAssistant(QObject* parent)
        : QObject(parent)
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
        sleep(5);
        emit upgradeFinished(2);
    }

} // namespace BLL
} // namespace ikoOSKAR
