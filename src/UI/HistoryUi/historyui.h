#ifndef IKOOSKAR_UI_HISTORYUI_H
#define IKOOSKAR_UI_HISTORYUI_H

#include "UI/Common/module.h"
#include <QWidget>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class HistoryUi;
    }

    class HistoryUi : public Common::Module {
        Q_OBJECT

    public:
        static HistoryUi* getInstance();
        const QString* getName();
        const QString* getDescription();
        ~HistoryUi();

    private:
        Ui::HistoryUi* ui;
        inline static HistoryUi* instance;
        explicit HistoryUi(QWidget* parent = nullptr);
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_HISTORYUI_H
