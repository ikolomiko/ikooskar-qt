#include "nsresultsui.h"
#include "ui_nsresultsui.h"
#include "xlsxbutton.h"

namespace ikoOSKAR {
namespace UI {

    NSResultsUi::NSResultsUi(const QString& examName, const QString& examDate, const QString& pathClassLists, const QString& pathHallLayouts, QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::NSResultsUi)
    {
        ui->setupUi(this);
        ui->lblName->setText(examName);
        ui->lblDate->setText(examDate);

        ui->frmLeft->layout()->addWidget(new XlsxButton("Oturma Planları", pathHallLayouts));
        ui->frmRight->layout()->addWidget(new XlsxButton("Sınıf Karma Listeleri", pathClassLists));
    }

    NSResultsUi::~NSResultsUi()
    {
        delete ui;
    }

} // namespace UI
} // namespace ikoOSKAR
