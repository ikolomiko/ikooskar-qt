#ifndef IKOOSKAR_BLL_UPGRADEASSISTANT_H
#define IKOOSKAR_BLL_UPGRADEASSISTANT_H

#include <QObject>
#include <QRunnable>
#include <QTemporaryDir>

namespace ikoOSKAR {
namespace BLL {

    class UpgradeAssistant : public QObject, public QRunnable {
        Q_OBJECT

    private:
        QString oldDbPath;
        QTemporaryDir* tempDir = nullptr;

        void findOldDatabaseFile();
        QByteArray convertToZippedSqlite();
        QByteArray unzip(const QString& zipPath);
        QString saveTemp(const QByteArray& bytes, const QString& name);
        int modifyDatabase(const QString& path);

    public:
        explicit UpgradeAssistant();
        ~UpgradeAssistant();
        bool willUpgrade();
        void run() override;

    signals:
        void upgradeFinished(int nStudents);
        void error(const QString& errorMessage);
    };

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_UPGRADEASSISTANT_H
