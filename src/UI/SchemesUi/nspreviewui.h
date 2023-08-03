#ifndef IKOOSKAR_UI_NSPREVIEWUI_H
#define IKOOSKAR_UI_NSPREVIEWUI_H

#include "Shared/hall.h"
#include "Shared/student.h"
#include <QWidget>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class NSPreviewUi;
    }

    class NSPreviewUi : public QWidget {
        Q_OBJECT

    public:
        explicit NSPreviewUi(QWidget* parent = nullptr);
        ~NSPreviewUi();

    private:
        Ui::NSPreviewUi* ui;
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_NSPREVIEWUI_H
