#ifndef IKOOSKAR_UI_XLSXBUTTON_H
#define IKOOSKAR_UI_XLSXBUTTON_H

#include <QWidget>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class XlsxButton;
    }

    class XlsxButton : public QWidget {
        Q_OBJECT

    public:
        explicit XlsxButton(const QString& name, const QString& filePath, QWidget* parent = nullptr);
        ~XlsxButton();

    private slots:
        void on_btnXlsxFile_clicked();

    private:
        Ui::XlsxButton* ui;
        QString filePath;
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_XLSXBUTTON_H
