#ifndef IKOOSKAR_UI_WELCOMEUI_H
#define IKOOSKAR_UI_WELCOMEUI_H

#include <QWidget>

namespace ikoOSKAR {
namespace UI {
    namespace Ui {
        class WelcomeUi;
    }

    class WelcomeUi : public QWidget {
        Q_OBJECT

    public:
        static WelcomeUi* getInstance();
        ~WelcomeUi();

    private:
        Ui::WelcomeUi* ui;
        inline static WelcomeUi* instance;
        explicit WelcomeUi(QWidget* parent = nullptr);
    };
} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_WELCOMEUI_H
