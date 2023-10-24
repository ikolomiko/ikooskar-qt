#ifndef IKOOSKAR_BLL_LICENSESTATUS_H
#define IKOOSKAR_BLL_LICENSESTATUS_H

#include <QPixmap>
#include <QString>

namespace ikoOSKAR {
namespace BLL {
    class LicenseStatus {
    public:
        QString text;
        QString imagePath;
        QString style;

        LicenseStatus(QString text, QString imagePath, QString style);
        QPixmap image();
        bool operator==(const LicenseStatus& other) const;

        static const LicenseStatus Activated;
        static const LicenseStatus Demo;
        static const LicenseStatus EndOfDemo;
    };
}
}
#endif // IKOOSKAR_BLL_LICENSESTATUS_H
