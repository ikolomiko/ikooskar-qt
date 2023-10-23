#include "app.h"
#include "UI/AuthenticatorWindow/authenticatorwindow.h"
#include "UI/ErrorUi/errorui.h"
#include "UI/MainWindow/mainwindow.h"

#include <QFile>
#include <QProcess>
#include <QTranslator>
#include <QtConcurrent/QtConcurrentRun>

namespace ikoOSKAR {

App::App(int argc, char* argv[])
    : QApplication(argc, argv)
{
    auto translator = new QTranslator();
    if (translator->load(":/qtbase_tr.qm")) {
        installTranslator(translator);
    }

    QFile styles(":/styles.qss");
    styles.open(QFile::ReadOnly);
    setStyleSheet(styles.readAll());
    styles.close();

    splash = new QSplashScreen(QPixmap(":/splash.png"));
    splash->showMessage("iko Ortak Sınav Karma Sistemi v4", Qt::AlignBottom | Qt::AlignLeft);
    splash->show();

    authenticator = new BLL::Authenticator();
    connect(authenticator, &BLL::Authenticator::success, this, &App::handleAuthSuccess);
    connect(authenticator, &BLL::Authenticator::error, this, &App::handleAuthError);

    (void)QtConcurrent::run([&]() {
        authenticator->login();
    });
}

void App::handleAuthSuccess(const QString& message)
{
    // Success message is ignored on logins
    (void)message;

    auto mainWindow = new UI::MainWindow();
    mainWindow->show();
    splash->finish(mainWindow);

    disconnect(authenticator, &ikoOSKAR::BLL::Authenticator::success, this, &App::handleAuthSuccess);
    disconnect(authenticator, &ikoOSKAR::BLL::Authenticator::error, this, &App::handleAuthError);
}

void App::handleAuthError(const QString& errorMessage)
{
    // Ignore empty error messages
    if (!errorMessage.isEmpty()) {
        const QString errorTitle = "Lisans Senkronizasyonunda Hata Oluştu";
        UI::ErrorUi(errorTitle).displayMessage(errorMessage);
    }

    auto authWindow = new UI::AuthenticatorWindow(authenticator);
    connect(authWindow, &UI::AuthenticatorWindow::restartApp, this, &App::handleRestart);
    authWindow->show();
    splash->finish(authWindow);

    disconnect(authenticator, &ikoOSKAR::BLL::Authenticator::success, this, &App::handleAuthSuccess);
    disconnect(authenticator, &ikoOSKAR::BLL::Authenticator::error, this, &App::handleAuthError);
}

void App::handleRestart()
{
    QProcess::startDetached(applicationFilePath());
    delete authenticator;
    delete splash;

    // This makes the program crash, but everything else just works exactly as intended.
    this->deleteLater();

    // A more proper way of restarting the app would be like this:
    // https://stackoverflow.com/a/6356216/9209685
    // However, this causes some weird graphical glitches on Windows that I couldn't fix.
    // So, I just decided to use this ugly hack instead.
}

} // namespace ikoOSKAR
