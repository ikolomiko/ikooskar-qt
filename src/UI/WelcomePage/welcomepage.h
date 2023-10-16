#ifndef IKOOSKAR_UI_WELCOMEPAGE_H
#define IKOOSKAR_UI_WELCOMEPAGE_H

#include "UI/Common/page.h"
#include <QWidget>

namespace ikoOSKAR {
namespace UI {
    namespace Ui {
        class WelcomePage;
    }

    class WelcomePage : public Common::Page {
        Q_OBJECT

    public:
        static WelcomePage* getInstance();
        const QString* getName();
        const QString* getDescription();
        ~WelcomePage();

    private:
        Ui::WelcomePage* ui;
        inline static WelcomePage* instance;
        explicit WelcomePage(QWidget* parent = nullptr);
    };
} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_WELCOMEPAGE_H
