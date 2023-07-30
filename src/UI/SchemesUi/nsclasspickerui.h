#ifndef IKOOSKAR_UI_NSCLASSPICKERUI_H
#define IKOOSKAR_UI_NSCLASSPICKERUI_H

#include <QStringListModel>
#include <QWidget>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class NSClassPickerUi;
    }

    class NSClassPickerUi : public QWidget {
        Q_OBJECT

    public:
        explicit NSClassPickerUi(QWidget* parent = nullptr);
        ~NSClassPickerUi();
        QList<QString> getAttendingClasses();

    private slots:
        void on_btnAddClass_clicked();

        void on_btnRmClass_clicked();

    private:
        Ui::NSClassPickerUi* ui;
        QStringListModel* attendingModel;
        QStringListModel* nonAttendingModel;

        void updateSelectedClasses();
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_NSCLASSPICKERUI_H
