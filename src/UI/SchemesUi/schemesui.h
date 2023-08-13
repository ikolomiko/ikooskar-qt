#ifndef IKOOSKAR_UI_SCHEMESUI_H
#define IKOOSKAR_UI_SCHEMESUI_H

#include "UI/Common/module.h"
#include <QWidget>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class SchemesUi;
    }

    class SchemesUi : public Common::Module {
        Q_OBJECT

    public:
        static SchemesUi* getInstance();
        const QString* getName();
        const QString* getDescription();
        ~SchemesUi();

    private slots:
        void on_btnNewScheme_clicked();

    private:
        Ui::SchemesUi* ui;
        inline static SchemesUi* instance;
        explicit SchemesUi(QWidget* parent = nullptr);
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_SCHEMESUI_H
