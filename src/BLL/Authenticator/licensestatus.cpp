#include "licensestatus.h"

namespace ikoOSKAR {
namespace BLL {
    const LicenseStatus LicenseStatus::Activated("Etkinleştirildi", ":/check.png", "");
    const LicenseStatus LicenseStatus::Demo("Ücretsiz deneme etkinleştirildi", ":/info.png", "background-color: yellow");
    const LicenseStatus LicenseStatus::EndOfDemo("Ücretsiz deneme haklarınız tükenmiştir", ":/remove.png", "background-color: red");

    LicenseStatus::LicenseStatus(QString text, QString imagePath, QString style)
        : text(text)
        , imagePath(imagePath)
        , style(style)
    {
    }

    QPixmap LicenseStatus::image()
    {
        return QPixmap(imagePath).scaled(22, 22, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    }

    bool LicenseStatus::operator==(const LicenseStatus& other) const
    {
        return text == other.text;
    }
}
}
