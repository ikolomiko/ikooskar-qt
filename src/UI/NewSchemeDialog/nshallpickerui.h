#ifndef IKOOSKAR_UI_NSHALLPICKERUI_H
#define IKOOSKAR_UI_NSHALLPICKERUI_H

#include <QStringListModel>
#include <QWidget>

namespace ikoOSKAR {
namespace UI {

    namespace Ui {
        class NSHallPickerUi;
    }

    class NSHallPickerUi : public QWidget {
        Q_OBJECT

    public:
        explicit NSHallPickerUi(QWidget* parent = nullptr);
        ~NSHallPickerUi();
        QList<QString> getSelectedHalls();

    private slots:
        void on_btnAddHall_clicked();

        void on_btnRmHall_clicked();

    private:
        Ui::NSHallPickerUi* ui;
        QStringListModel* includedModel;
        QStringListModel* excludedModel;

        void updateSelectedHalls();
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_NSHALLPICKERUI_H
