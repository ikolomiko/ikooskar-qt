#include "mifilepickerui.h"
#include "ui_mifilepickerui.h"

namespace ikoOSKAR {
namespace UI {

    MIFilePickerUi::MIFilePickerUi(QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::MIFilePickerUi)
    {
        ui->setupUi(this);
        btnOpenFile = ui->btnOpenFile;
        btnHelpExcel = ui->btnHelpExcel;
    }

    MIFilePickerUi::~MIFilePickerUi()
    {
        delete ui;
    }

} // namespace UI
} // namespace ikoOSKAR
