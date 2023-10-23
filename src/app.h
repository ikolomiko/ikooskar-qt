#ifndef IKOOSKAR_APP_H
#define IKOOSKAR_APP_H

#include "BLL/Authenticator/authenticator.h"
#include <QApplication>
#include <QSplashScreen>

namespace ikoOSKAR {

class App : public QApplication {
    Q_OBJECT
private:
    BLL::Authenticator* authenticator;
    QSplashScreen* splash;

    void handleAuthSuccess(const QString& message);
    void handleAuthError(const QString& errorMessage);
    void handleRestart();

public:
    App(int argc, char* argv[]);
};

} // namespace ikoOSKAR

#endif // IKOOSKAR_APP_H
