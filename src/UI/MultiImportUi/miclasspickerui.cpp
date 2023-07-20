#include "miclasspickerui.h"
#include "ui_miclasspickerui.h"

namespace ikoOSKAR {
namespace UI {

MIClassPickerUi::MIClassPickerUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MIClassPickerUi)
{
    ui->setupUi(this);
}

MIClassPickerUi::~MIClassPickerUi()
{
    delete ui;
}

} // namespace UI
} // namespace ikoOSKAR
