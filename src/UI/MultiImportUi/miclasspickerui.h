#ifndef IKOOSKAR_UI_MICLASSPICKERUI_H
#define IKOOSKAR_UI_MICLASSPICKERUI_H

#include <QPushButton>
#include <QWidget>

namespace ikoOSKAR {
namespace UI {
    namespace Ui {
        class MIClassPickerUi;
    }

    class MIClassPickerUi : public QWidget {
        Q_OBJECT

    public:
        QPushButton* btnPrev;

        explicit MIClassPickerUi(int nParsedStudents, QWidget* parent = nullptr);
        ~MIClassPickerUi();

    signals:
        void selectedGradeAndSection(int grade, const QString& section);

    private slots:
        void on_btnNext_clicked();

    private:
        Ui::MIClassPickerUi* ui;
    };

} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_MICLASSPICKERUI_H
