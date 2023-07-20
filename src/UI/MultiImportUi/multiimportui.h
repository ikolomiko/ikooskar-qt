#ifndef IKOOSKAR_UI_MULTIIMPORTUI_H
#define IKOOSKAR_UI_MULTIIMPORTUI_H

#include "BLL/MultiImportHelper/multiimporthelper.h"
#include "UI/Common/spinner.h"
#include <QDialog>
#include <QList>

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

    public slots:
        void handleParsedXls(QList<Shared::Student*>*);

    private slots:
        void btnOpenFile_clicked();
        void btnHelpExcel_clicked();

    private:
        Ui::MultiImportUi* ui;
        Common::Spinner* spinner;

        const QString* pickXlsFile();
        void showSpinner();
        void hideSpinner();
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_MULTIIMPORTUI_H
