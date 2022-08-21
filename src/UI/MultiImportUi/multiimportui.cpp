#include "multiimportui.h"
#include "ui_multiimportui.h"

namespace ikoOSKAR {
namespace UI {

MultiImportUi::MultiImportUi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultiImportUi)
{
    ui->setupUi(this);
}

MultiImportUi::~MultiImportUi()
{
    delete ui;
}

} // namespace UI
} // namespace ikoOSKAR
