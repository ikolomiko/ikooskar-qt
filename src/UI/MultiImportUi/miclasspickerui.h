#ifndef IKOOSKAR_UI_MICLASSPICKERUI_H
#define IKOOSKAR_UI_MICLASSPICKERUI_H

#include <QWidget>

namespace ikoOSKAR {
namespace UI {

namespace Ui {
class MIClassPickerUi;
}

class MIClassPickerUi : public QWidget
{
    Q_OBJECT

public:
    explicit MIClassPickerUi(QWidget *parent = nullptr);
    ~MIClassPickerUi();

private:
    Ui::MIClassPickerUi *ui;
};


} // namespace UI
} // namespace ikoOSKAR
#endif // IKOOSKAR_UI_MICLASSPICKERUI_H
