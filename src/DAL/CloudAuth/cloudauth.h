#ifndef IKOOSKAR_DAL_CLOUDAUTH_H
#define IKOOSKAR_DAL_CLOUDAUTH_H

#include "cloudauthresponse.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

namespace ikoOSKAR {
namespace DAL {

    class CloudAuth {
    private:
        const QUrl baseUrl = { "http://ikooskar.com.tr" };

        QString getMacAddress();
        CloudAuthResponse sendRequest(QUrl url);

    public:
        bool isOnline();
        CloudAuthResponse setDemoRemainings(int remainings);
        CloudAuthResponse activateLicense(const QString& license);
    };

} // namespace DAL
} // namespace ikoOSKAR

#endif // IKOOSKAR_DAL_CLOUDAUTH_H
