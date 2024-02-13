#include "app.h"
#include "UI/AuthenticatorWindow/authenticatorwindow.h"
#include "UI/ErrorUi/errorui.h"
#include "UI/MainWindow/mainwindow.h"

#include <QFile>
#include <QProcess>
#include <QTranslator>
#include <QtConcurrent/QtConcurrentRun>
#include <QFontDatabase>
#include <QStyleFactory>
#include <QPalette>

namespace ikoOSKAR {

App::App(int argc, char* argv[])
    : QApplication(argc, argv)
{
    setApplicationDisplayName("ikoOSKAR - iko Ortak Sınav Karma Sistemi");

    auto translator = new QTranslator();
    if (translator->load(":/qtbase_tr.qm")) {
        installTranslator(translator);
    }

    QFontDatabase::addApplicationFont(":/NotoSans.ttf");

    QFile styles(":/styles.qss");
    styles.open(QFile::ReadOnly);
    setStyleSheet(styles.readAll());
    styles.close();

    setPalette(fusionLight());

    splash = new QSplashScreen(QPixmap(":/splash.png"));
    QString splashText = QString("%1 v%2").arg(applicationDisplayName(), applicationVersion());
    splash->showMessage(splashText, Qt::AlignBottom | Qt::AlignLeft);
    splash->show();

    authenticator = BLL::Authenticator::getInstance();
    connect(authenticator, &BLL::Authenticator::success, this, &App::handleAuthSuccess);
    connect(authenticator, &BLL::Authenticator::error, this, &App::handleAuthError);

    (void)QtConcurrent::run([&]() {
        authenticator->login();
    });
}

// Copied this entire method from qt6/qtbase/src/gui/kernel/qplatformtheme.cpp
QPalette App::fusionLight()
{
    const QColor windowText = Qt::black;
    const QColor backGround = QColor(239, 239, 239);
    const QColor light = backGround.lighter(150);
    const QColor mid = (backGround.darker(130));
    const QColor midLight = mid.lighter(110);
    const QColor base = Qt::white;
    const QColor disabledBase(backGround);
    const QColor dark = backGround.darker(150);
    const QColor darkDisabled = QColor(209, 209, 209).darker(110);
    const QColor text = Qt::black;
    const QColor highlight = QColor(48, 140, 198);
    const QColor hightlightedText = Qt::white;
    const QColor disabledText = QColor(190, 190, 190);
    const QColor button = backGround;
    const QColor shadow = dark.darker(135);
    const QColor disabledShadow = shadow.lighter(150);
    const QColor disabledHighlight(145, 145, 145);
    QColor placeholder = text;
    placeholder.setAlpha(128);
    QPalette fusionPalette(windowText, backGround, light, dark, mid, text, base);
    fusionPalette.setBrush(QPalette::Midlight, midLight);
    fusionPalette.setBrush(QPalette::Button, button);
    fusionPalette.setBrush(QPalette::Shadow, shadow);
    fusionPalette.setBrush(QPalette::HighlightedText, hightlightedText);
    fusionPalette.setBrush(QPalette::Disabled, QPalette::Text, disabledText);
    fusionPalette.setBrush(QPalette::Disabled, QPalette::WindowText, disabledText);
    fusionPalette.setBrush(QPalette::Disabled, QPalette::ButtonText, disabledText);
    fusionPalette.setBrush(QPalette::Disabled, QPalette::Base, disabledBase);
    fusionPalette.setBrush(QPalette::Disabled, QPalette::Dark, darkDisabled);
    fusionPalette.setBrush(QPalette::Disabled, QPalette::Shadow, disabledShadow);
    fusionPalette.setBrush(QPalette::Active, QPalette::Highlight, highlight);
    fusionPalette.setBrush(QPalette::Inactive, QPalette::Highlight, highlight);
    fusionPalette.setBrush(QPalette::Disabled, QPalette::Highlight, disabledHighlight);
#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
    fusionPalette.setBrush(QPalette::Active, QPalette::Accent, highlight);
    fusionPalette.setBrush(QPalette::Inactive, QPalette::Accent, highlight);
    fusionPalette.setBrush(QPalette::Disabled, QPalette::Accent, disabledHighlight);
#endif
    fusionPalette.setBrush(QPalette::PlaceholderText, placeholder);

    return fusionPalette;
}

void App::handleAuthSuccess(const QString& message)
{
    // Success message is ignored on logins
    (void)message;

    authenticator->disconnect();

    auto mainWindow = new UI::MainWindow();
    mainWindow->show();
    splash->finish(mainWindow);
}

void App::handleAuthError(const QString& errorMessage)
{
    // Ignore empty error messages
    if (!errorMessage.isEmpty()) {
        const QString errorTitle = "Lisans Senkronizasyonunda Hata Oluştu";
        UI::ErrorUi(errorTitle).displayMessage(errorMessage);
    }

    auto authWindow = new UI::AuthenticatorWindow();
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
