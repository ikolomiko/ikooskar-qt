#ifndef IKOOSKAR_UI_ABOUTUI_H
#define IKOOSKAR_UI_ABOUTUI_H

#include "UI/Common/module.h"
#include <QWidget>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class AboutUi;
    }

    class AboutUi : public Common::Module {
        Q_OBJECT

    public:
        static AboutUi* getInstance();
        const QString* getName();
        const QString* getDescription();
        ~AboutUi();

    private:
        Ui::AboutUi* ui;
        inline static AboutUi* instance;
        explicit AboutUi(QWidget* parent = nullptr);
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_ABOUTUI_H
