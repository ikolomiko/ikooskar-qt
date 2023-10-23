#include "localauth.h"

namespace ikoOSKAR {
namespace DAL {

    bool LocalAuth::isDemo()
    {
        return settings.value("isDemo", false).toBool();
    }

    void LocalAuth::setDemo(bool isDemo)
    {
        settings.setValue("isDemo", isDemo);
    }

    int LocalAuth::getDemoRemainings()
    {
        return settings.value("demoRemainings", 3).toInt();
    }

    void LocalAuth::setDemoRemainings(int remainings)
    {
        settings.setValue("demoRemainings", remainings);
    }

    QString LocalAuth::getSerial()
    {
        return settings.value("serial", "").toString();
    }

    void LocalAuth::setSerial(const QString& serial)
    {
        settings.setValue("serial", serial);
    }

} // namespace DAL
} // namespace ikoOSKAR
