#ifndef IKOOSKAR_UI_ABOUTPAGE_H
#define IKOOSKAR_UI_ABOUTPAGE_H

#include "UI/Common/page.h"
#include <QWidget>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class AboutPage;
    }

    class AboutPage : public Common::Page {
        Q_OBJECT

    public:
        static AboutPage* getInstance();
        const QString* getName();
        const QString* getDescription();
        ~AboutPage();

    private:
        Ui::AboutPage* ui;
        inline static AboutPage* instance;
        explicit AboutPage(QWidget* parent = nullptr);
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_ABOUTPAGE_H
