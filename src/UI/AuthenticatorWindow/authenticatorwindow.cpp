#include "authenticatorwindow.h"
#include "UI/Common/contactwidget.h"
#include "UI/ErrorUi/errorui.h"
#include "ui_authenticatorwindow.h"
#include <QStatusBar>
#include <QtConcurrent/QtConcurrentRun>

namespace ikoOSKAR {
namespace UI {

    AuthenticatorWindow::AuthenticatorWindow(BLL::Authenticator* auth, QWidget* parent)
        : QMainWindow(parent)
        , ui(new Ui::AuthenticatorWindow)
        , authenticator(auth)
    {
        ui->setupUi(this);

        ui->lblBanner->setPixmap(QPixmap(":/banner.png").scaled(300, 60, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        statusBar()->showMessage("iko Ortak Sınav Karma Sistemi v4.0");
        statusBar()->setStyleSheet("margin: 3px");
        statusBar()->setSizeGripEnabled(false);

        auto website = new Common::ContactWidget(
            QPixmap(":/web.png"),
            "ikooskar.com.tr",
            "https://ikooskar.com.tr");
        auto email = new Common::ContactWidget(
            QPixmap(":/email.png"),
            "ikolomiko@gmail.com",
            "mailto:ikolomiko@gmail.com");

        ui->contactCards->layout()->addWidget(website);
        ui->contactCards->layout()->addWidget(email);

        connect(authenticator, &BLL::Authenticator::error, this, &AuthenticatorWindow::handleError);
        connect(authenticator, &BLL::Authenticator::success, this, &AuthenticatorWindow::handleSuccess);
    }

    AuthenticatorWindow::~AuthenticatorWindow()
    {
        delete ui;
    }

    void AuthenticatorWindow::on_btnActivateLicense_clicked()
    {
        setButtonsEnabled(false);

        (void)QtConcurrent::run([&]() {
            QString serial = ui->txtSerial->text().trimmed();
            authenticator->signupLicensed(serial);
        });
    }

    void AuthenticatorWindow::on_btnStartDemo_clicked()
    {
        setButtonsEnabled(false);
        (void)QtConcurrent::run([&]() {
            authenticator->signupDemo();
        });
    }

    void AuthenticatorWindow::handleError(const QString& errorMessage)
    {
        const QString errorTitle = "Aktivasyon Hatası";
        UI::ErrorUi(errorTitle).displayMessage(errorMessage);
        ui->txtSerial->clear();
        setButtonsEnabled(true);
    }

    void AuthenticatorWindow::handleSuccess(const QString& message)
    {
        const QString title = "Aktivasyon Başarılı";
        QMessageBox::information(QApplication::activeWindow(), title, message);
        emit restartApp();
    }

    void AuthenticatorWindow::setButtonsEnabled(bool isEnabled)
    {
        if (isEnabled) {
            ui->btnStartDemo->setText("Ücretsiz Deneme Sürümünü Başlat");
            ui->btnStartDemo->setStyleSheet("");
            ui->btnStartDemo->setEnabled(true);
            ui->btnActivateLicense->setText("Etkinleştir");
            ui->btnActivateLicense->setStyleSheet("");
            ui->btnActivateLicense->setEnabled(true);
        } else {
            ui->btnStartDemo->setText("Lütfen Bekleyiniz...");
            ui->btnStartDemo->setStyleSheet("color: #777777");
            ui->btnStartDemo->setEnabled(false);
            ui->btnActivateLicense->setText("Lütfen Bekleyiniz...");
            ui->btnActivateLicense->setStyleSheet("color: #777777");
            ui->btnActivateLicense->setEnabled(false);
        }
    }

} // namespace UI
} // namespace ikoOSKAR
