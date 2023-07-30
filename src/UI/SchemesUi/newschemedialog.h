#ifndef IKOOSKAR_UI_NEWSCHEMEDIALOG_H
#define IKOOSKAR_UI_NEWSCHEMEDIALOG_H

#include "UI/Common/twobuttonnav.h"
#include <QDialog>

namespace ikoOSKAR {
namespace UI {
    namespace Ui {
        class NewSchemeDialog;
    }

    class NewSchemeDialog : public QDialog {
        Q_OBJECT

    public:
        explicit NewSchemeDialog(QWidget* parent = nullptr);
        ~NewSchemeDialog();

    private:
        enum PageState {
            EXAM_INFO = 0,
            CLASS_PICKER,
            HALL_PICKER,
            PREVIEW,
            SPINNER,
            RESULTS
        };

        Ui::NewSchemeDialog* ui;
        Common::TwoButtonNav* nav;
        PageState page;

        void prevPage();
        void nextPage();
    };
} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_NEWSCHEMEDIALOG_H
