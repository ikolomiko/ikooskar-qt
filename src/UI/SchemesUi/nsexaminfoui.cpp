#include "nsexaminfoui.h"
#include "ui_nsexaminfoui.h"

namespace ikoOSKAR {
namespace UI {

    NSExamInfoUi::NSExamInfoUi(QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::NSExamInfoUi)
    {
        ui->setupUi(this);
        ui->dateEdit->setDate(QDate::currentDate());
    }

    QString NSExamInfoUi::getExamName()
    {
        return ui->cmbName->currentText();
    }

    QDate NSExamInfoUi::getExamDate()
    {
        return ui->dateEdit->date();
    }

    NSExamInfoUi::~NSExamInfoUi()
    {
        delete ui;
    }

} // namespace UI
} // namespace ikoOSKAR
