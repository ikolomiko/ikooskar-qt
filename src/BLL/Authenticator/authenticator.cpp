#include "authenticator.h"

namespace ikoOSKAR {
namespace BLL {
    Authenticator::Authenticator() { }

    Authenticator* Authenticator::getInstance()
    {
        if (instance == nullptr) {
            instance = new Authenticator();
        }
        return instance;
    }

    void Authenticator::login()
    {
        // Login steps for demo users
        if (localAuth.isDemo()) {
            if (cloudAuth.isOnline()) {
                // Proper signal gets sent in synchronizeDemo()
                synchronizeDemo();
            } else {
                if (localAuth.getDemoRemainings() > 0) {
                    emit success();
                } else {
                    emit error("Ücretsiz deneme haklarınız tükenmiştir. Eğer ikoOSKAR'dan memnun kaldıysanız "
                               "ve programı kullanmaya devam etmek istiyorsanız lütfen ikolomiko@gmail.com "
                               "e-posta adresinden iletişime geçip bir lisans anahtarı satın alınız.");
                }
            }
            return;
        }

        // Login steps for non-demo users

        // Check if the serial is empty
        QString serial = localAuth.getSerial();
        if (serial.isEmpty()) {
            emit error();
            return;
        }

        // Check if the serial is valid if both the server and the client are online
        if (cloudAuth.isOnline()) {
            // Proper signal gets sent in synchronizeLicense()
            synchronizeLicense(serial);
        } else {
            int remainings = localAuth.getDemoRemainings();
            if (remainings > 0) {
                emit success();
            } else {
                emit error();
            }
        }
    }

    void Authenticator::signupLicensed(QString serial)
    {
        if (serial.isEmpty()) {
            emit error("Lütfen lisans anahtarınızı giriniz.");
            return;
        }

        if (serial.size() < 3) {
            emit error("Lisans anahtarı 3 harften kısa olamaz.");
            return;
        }

        if (!cloudAuth.isOnline()) {
            emit error("Bilgisayarınız internete bağlı değil. Lütfen internete bağlanıp tekrar deneyiniz.");
            return;
        }

        // Proper signal gets sent in synchronizeLicense()
        bool isSignupSuccessful = synchronizeLicense(serial);
        if (isSignupSuccessful) {
            localAuth.setSerial(serial);
            localAuth.setDemo(false);
        }
    }

    void Authenticator::signupDemo()
    {
        if (!cloudAuth.isOnline()) {
            emit error("Bilgisayarınız internete bağlı değil. Lütfen internete bağlanıp tekrar deneyiniz.");
            return;
        }

        // Proper signal gets sent in synchronizeDemo()
        bool isSignupSuccessful = synchronizeDemo();
        localAuth.setSerial("");
        localAuth.setDemo(isSignupSuccessful);
    }

    int Authenticator::getDemoRemainings()
    {
        return localAuth.getDemoRemainings();
    }

    void Authenticator::decreaseDemoRemainingsByOne()
    {
        int current = localAuth.getDemoRemainings();
        localAuth.setDemoRemainings(current - 1);
        synchronizeDemo();
        emit demoUpdated();
    }

    bool Authenticator::isDemo()
    {
        return localAuth.isDemo();
    }

    QString Authenticator::getSerial()
    {
        return localAuth.getSerial();
    }

    const LicenseStatus Authenticator::getLicenseStatus()
    {
        if (isDemo()) {
            int remainings = getDemoRemainings();
            if (remainings > 0) {
                return LicenseStatus::Demo;
            } else {
                return LicenseStatus::EndOfDemo;
            }
        } else {
            return LicenseStatus::Activated;
        }
    }

    bool Authenticator::synchronizeLicense(const QString& serial)
    {
        auto response = cloudAuth.activateLicense(serial);

        switch (response.statusCode) {
        case CloudAuthResponse::REACTIVATED:
            emit success("Lisans anahtarı yeniden etkinleştirildi.");
            return true;

        case CloudAuthResponse::ACTIVATED:
            emit success("Lisans anahtarı etkinleştirildi.");
            return true;

        case CloudAuthResponse::BAD_REQUEST:
            emit error("Lisans anahtarı etkinleştirilemedi: Hata kodu 400, hatalı istek.");
            return false;

        case CloudAuthResponse::UNAUTHORIZED:
            emit error("Lisans anahtarı etkinleştirilemedi: Hata kodu 401, yetkisiz erişim.");
            return false;

        case CloudAuthResponse::SERIAL_NOT_FOUND:
            emit error("Lisans anahtarı etkinleştirilemedi: Hata kodu 404, lisans anahtarı bulunamadı.");
            return false;

        default:
            emit error("Lisans anahtarı etkinleştirilemedi: Diğer hata: " + response.body);
            return false;
        }
    }

    bool Authenticator::synchronizeDemo()
    {
        int localRemainings = localAuth.getDemoRemainings();
        auto response = cloudAuth.setDemoRemainings(localRemainings);
        int remoteRemainings;

        switch (response.statusCode) {
        case CloudAuthResponse::REACTIVATED:
        case CloudAuthResponse::ACTIVATED:
        case CloudAuthResponse::END_OF_DEMO:
            remoteRemainings = response.getDemoRemainings();
            localAuth.setDemoRemainings(remoteRemainings);
            if (remoteRemainings > 0) {
                emit success("Deneme sürümü etkinleştirildi.");
                return true;
            } else {
                emit error("Ücretsiz deneme haklarınız tükenmiştir. Eğer ikoOSKAR'dan memnun kaldıysanız "
                           "ve programı kullanmaya devam etmek istiyorsanız lütfen ikolomiko@gmail.com "
                           "e-posta adresinden iletişime geçip bir lisans anahtarı satın alınız.");
                return false;
            }
        case CloudAuthResponse::BAD_REQUEST:
            emit error("Deneme sürümü etkinleştirilemedi: Hata kodu 400, hatalı istek.");
            return false;
        default:
            emit error("Deneme sürümü etkinleştirilemedi: Diğer hata: " + response.body);
            return false;
        }
    }

} // namespace BLL
} // namespace ikoOSKAR
