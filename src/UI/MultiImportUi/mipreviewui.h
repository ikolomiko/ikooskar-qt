#ifndef IKOOSKAR_UI_MIPREVIEWUI_H
#define IKOOSKAR_UI_MIPREVIEWUI_H

#include "Shared/student.h"
#include <QList>
#include <QPushButton>
#include <QWidget>

namespace ikoOSKAR {
namespace UI {
    using namespace Shared;
    namespace Ui {
        class MIPreviewUi;
    }

    class MIPreviewUi : public QWidget {
        Q_OBJECT

    public:
        QPushButton* btnPrev;
        QPushButton* btnConfirm;

        explicit MIPreviewUi(QList<Student*>* students, QWidget* parent = nullptr);
        ~MIPreviewUi();

    private:
        Ui::MIPreviewUi* ui;
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_MIPREVIEWUI_H
