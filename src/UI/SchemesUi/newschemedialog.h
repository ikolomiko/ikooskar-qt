#ifndef IKOOSKAR_UI_NEWSCHEMEDIALOG_H
#define IKOOSKAR_UI_NEWSCHEMEDIALOG_H

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
        Ui::NewSchemeDialog* ui;
    };
} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_NEWSCHEMEDIALOG_H
