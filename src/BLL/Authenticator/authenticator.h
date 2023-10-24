#ifndef IKOOSKAR_BLL_AUTHENTICATOR_H
#define IKOOSKAR_BLL_AUTHENTICATOR_H

#include "DAL/CloudAuth/cloudauth.h"
#include "DAL/LocalAuth/localauth.h"

namespace ikoOSKAR {
namespace BLL {
    using namespace ikoOSKAR::DAL;

    class Authenticator : public QObject {
        Q_OBJECT
    private:
        CloudAuth cloudAuth;
        LocalAuth localAuth;

    public:
        static Authenticator* getInstance();
        void login();
        void signupLicensed(QString serial);
        void signupDemo();

    private:
        inline static Authenticator* instance = nullptr;

        Authenticator();
        bool synchronizeLicense(const QString& serial);
        bool synchronizeDemo();

    signals:
        void success(const QString& message = "");
        void error(const QString& errorMessage = "");
    };

} // namespace BLL
} // namespace ikoOSKAR

#endif // IKOOSKAR_BLL_AUTHENTICATOR_H
