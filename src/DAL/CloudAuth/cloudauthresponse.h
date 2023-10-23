#ifndef IKOOSKAR_DAL_CLOUDAUTHRESPONSE_H
#define IKOOSKAR_DAL_CLOUDAUTHRESPONSE_H

#include <QNetworkReply>
#include <QString>

namespace ikoOSKAR {
namespace DAL {

    class CloudAuthResponse {
    public:
        enum StatusCode {
            REACTIVATED = 200,
            ACTIVATED = 201,
            BAD_REQUEST = 400,
            UNAUTHORIZED = 401, // Both MAC addresses and IP addresses don't match
            END_OF_DEMO = 403,
            SERIAL_NOT_FOUND = 404,
            SERVER_ERROR = 500,
            OTHER_ERROR = 999
        };

        StatusCode statusCode;
        QString body;

        CloudAuthResponse(QNetworkReply* reply)
        {
            int rawStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            switch (rawStatusCode) {
            case 200:
            case 201:
            case 400:
            case 401:
            case 403:
            case 404:
            case 500:
                statusCode = (StatusCode)rawStatusCode;
                break;
            default:
                statusCode = OTHER_ERROR;
                break;
            }

            body = reply->readAll().trimmed();
        }

        int getDemoRemainings()
        {
            bool success;
            int result = body.toInt(&success);
            if (success) {
                return result;
            } else {
                return -1;
            }
        }
    };
}
}

#endif // IKOOSKAR_DAL_CLOUDAUTHRESPONSE_H
