#ifndef IKOOSKAR_UI_SCHEMEGENERATORUI_H
#define IKOOSKAR_UI_SCHEMEGENERATORUI_H

#include "UI/Common/module.h"
#include <QWidget>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class SchemeGeneratorUi;
    }

    class SchemeGeneratorUi : public Common::Module {
        Q_OBJECT

    public:
        static SchemeGeneratorUi* getInstance();
        const QString* getName();
        const QString* getDescription();
        ~SchemeGeneratorUi();

    private:
        Ui::SchemeGeneratorUi* ui;
        inline static SchemeGeneratorUi* instance;
        explicit SchemeGeneratorUi(QWidget* parent = nullptr);
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_SCHEMEGENERATORUI_H
