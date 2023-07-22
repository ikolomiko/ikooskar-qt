#ifndef IKOOSKAR_UI_MIFILEPICKERUI_H
#define IKOOSKAR_UI_MIFILEPICKERUI_H

#include <QPushButton>
#include <QWidget>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class MIFilePickerUi;
    }

    class MIFilePickerUi : public QWidget {
        Q_OBJECT

    public:
        explicit MIFilePickerUi(QWidget* parent = nullptr);
        ~MIFilePickerUi();

    private slots:
        void on_btnHelpExcel_clicked();

        void on_btnOpenFile_clicked();

    signals:
        void pickedXlsFile(QString* xlsFilePath);

    private:
        Ui::MIFilePickerUi* ui;
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_MIFILEPICKERUI_H
