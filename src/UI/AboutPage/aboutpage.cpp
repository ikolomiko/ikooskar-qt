#include "aboutpage.h"
#include "UI/Common/contactwidget.h"
#include "app.h"
#include "ui_aboutpage.h"
#include <QSettings>

namespace ikoOSKAR {
namespace UI {

    AboutPage::AboutPage(QWidget* parent)
        : Common::Page(parent)
        , ui(new Ui::AboutPage)
        , authenticator(BLL::Authenticator::getInstance())
    {
        ui->setupUi(this);
        name = new QString("Yardım ve İletişim");

        ui->lblIcon->setPixmap(QPixmap(":/Icon-WhiteBGx128.png").scaled(128, 128, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

        ui->lblName->setText(ikoOSKAR::App::longDisplayName);
        ui->lblVersion->setText("v" + ikoOSKAR::App::applicationVersion());

        auto website = new Common::ContactWidget(
            QPixmap(":/web.png"),
            "ikooskar.com.tr",
            "https://ikooskar.com.tr");
        auto email = new Common::ContactWidget(
            QPixmap(":/email.png"),
            "iletisim@ikooskar.com.tr",
            "mailto:iletisim@ikooskar.com.tr");
        ui->contactCards->layout()->addWidget(website);
        ui->contactCards->layout()->addWidget(email);

        refreshLicenseStatus();

        connect(authenticator, &BLL::Authenticator::demoUpdated, this, &AboutPage::refreshLicenseStatus);
    }

    void AboutPage::refreshLicenseStatus()
    {
        auto status = authenticator->getLicenseStatus();
        if (status == BLL::LicenseStatus::Activated) {
            ui->lblSerial->setText("Lisans Anahtarı: ");
            ui->txtSerial->setText(authenticator->getSerial());
        } else { // Demo or EndOfDemo
            int remainings = authenticator->getDemoRemainings();
            ui->lblSerial->setText("Kalan Deneme Hakları: ");
            ui->txtSerial->setText(QString::number(remainings));
        }

        ui->imgLicenseStatus->setPixmap(status.image());
        ui->lblLicenseStatus->setText(status.text);
    }

    AboutPage::~AboutPage()
    {
        delete ui;
    }

    AboutPage* AboutPage::getInstance()
    {
        if (instance == nullptr) {
            instance = new AboutPage();
        }
        return instance;
    }

    const QString* AboutPage::getDescription()
    {
        auto status = authenticator->getLicenseStatus();
        return new QString("Lisans durumu: " + status.text);
    }

} // namespace UI
} // namespace ikoOSKAR
