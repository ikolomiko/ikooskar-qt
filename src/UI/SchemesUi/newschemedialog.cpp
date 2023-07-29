#include "newschemedialog.h"
#include "ui_newschemedialog.h"

namespace ikoOSKAR {
namespace UI {

NewSchemeDialog::NewSchemeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewSchemeDialog)
{
    ui->setupUi(this);
}

NewSchemeDialog::~NewSchemeDialog()
{
    delete ui;
}

} // namespace UI
} // namespace ikoOSKAR
