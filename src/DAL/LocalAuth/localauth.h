#ifndef IKOOSKAR_DAL_LOCALAUTH_H
#define IKOOSKAR_DAL_LOCALAUTH_H

#include <QSettings>
#include <QString>

namespace ikoOSKAR {
namespace DAL {
    class LocalAuth {
    private:
        QSettings settings;

    public:
        bool isDemo();
        void setDemo(bool isDemo);
        int getDemoRemainings();
        void setDemoRemainings(int remainings);
        QString getSerial();
        void setSerial(const QString& serial);
    };

} // namespace DAL
} // namespace ikoOSKAR

#endif // IKOOSKAR_DAL_LOCALAUTH_H
