#ifndef IKOOSKAR_UI_MULTIIMPORTUI_H
#define IKOOSKAR_UI_MULTIIMPORTUI_H

#include "BLL/MultiImportHelper/multiimporthelper.h"
#include <QDialog>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class MultiImportUi;
    }

    class MultiImportUi : public QDialog {
        Q_OBJECT

    public:
        explicit MultiImportUi(QWidget* parent = nullptr);
        ~MultiImportUi();

    private slots:
        void on_btnOpenFile_clicked();

    private:
        Ui::MultiImportUi* ui;
        BLL::MultiImportHelper* bll;
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_MULTIIMPORTUI_H
