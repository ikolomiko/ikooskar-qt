#include "cloudauth.h"
#include <QEventLoop>
#include <QNetworkInterface>
#include <QNetworkReply>
#include <QUrlQuery>

namespace ikoOSKAR {
namespace DAL {

    QString CloudAuth::getMacAddress()
    {
        for (QNetworkInterface netInterface : QNetworkInterface::allInterfaces()) {
            // Return only the first non-loopback MAC Address
            if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
                return netInterface.hardwareAddress();
        }
        return QString();
    }

    CloudAuthResponse CloudAuth::sendRequest(QUrl url)
    {
        QNetworkAccessManager manager;
        QNetworkRequest request(url);
        QNetworkReply* reply = manager.get(request);

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        return { reply };
    }

    bool CloudAuth::isOnline()
    {
        QUrl url = baseUrl;
        url.setPath("/v4/ip");
        auto response = sendRequest(url);
        return response.statusCode == 200;
    }

    CloudAuthResponse CloudAuth::setDemoRemainings(int remainings)
    {
        QUrl url = baseUrl;
        url.setPath("/v4/demo");
        QUrlQuery urlQuery;
        urlQuery.addQueryItem("mac", getMacAddress());
        urlQuery.addQueryItem("remainings", QString::number(remainings));
        url.setQuery(urlQuery);

        auto response = sendRequest(url);
        return response;
    }

    CloudAuthResponse CloudAuth::activateLicense(const QString& serial)
    {
        QUrl url = baseUrl;
        url.setPath("/v4/license");
        QUrlQuery urlQuery;
        urlQuery.addQueryItem("mac", getMacAddress());
        urlQuery.addQueryItem("serial", serial.trimmed());
        url.setQuery(urlQuery);

        auto response = sendRequest(url);
        return response;
    }

} // namespace DAL
} // namespace ikoOSKAR
