#include "nsresultsui.h"
#include "ui_nsresultsui.h"

namespace ikoOSKAR {
namespace UI {

    NSResultsUi::NSResultsUi(const QString& examName, const QString& examDate, QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::NSResultsUi)
    {
        ui->setupUi(this);
        ui->lblName->setText(examName);
        ui->lblDate->setText(examDate);
    }

    NSResultsUi::~NSResultsUi()
    {
        delete ui;
    }

} // namespace UI
} // namespace ikoOSKAR
