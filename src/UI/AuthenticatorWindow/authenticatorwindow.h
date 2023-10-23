#ifndef IKOOSKAR_UI_AUTHENTICATORWINDOW_H
#define IKOOSKAR_UI_AUTHENTICATORWINDOW_H

#include "BLL/Authenticator/authenticator.h"
#include <QMainWindow>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class AuthenticatorWindow;
    }

    class AuthenticatorWindow : public QMainWindow {
        Q_OBJECT

    public:
        explicit AuthenticatorWindow(BLL::Authenticator* auth, QWidget* parent = nullptr);
        ~AuthenticatorWindow();

    private slots:
        void on_btnActivateLicense_clicked();

        void on_btnStartDemo_clicked();

        void handleError(const QString& errorMessage);

        void handleSuccess(const QString& message);

    private:
        Ui::AuthenticatorWindow* ui;
        BLL::Authenticator* authenticator;

        void setButtonsEnabled(bool isEnabled);

    signals:
        void restartApp();
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_AUTHENTICATORWINDOW_H
