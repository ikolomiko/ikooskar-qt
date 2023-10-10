#include "nsresultsui.h"
#include "ui_nsresultsui.h"
#include "xlsxbutton.h"

namespace ikoOSKAR {
namespace UI {

    NSResultsUi::NSResultsUi(const QString& examName, const QString& examDate, const QString& pathClassLists, const QString& pathHallLayouts, QWidget* parent, bool isHistorical)
        : QWidget(parent)
        , ui(new Ui::NSResultsUi)
    {
        ui->setupUi(this);
        ui->lblName->setText(examName);
        ui->lblDate->setText(examDate);

        auto btnLeft = new XlsxButton("Oturma Planları", pathHallLayouts);
        auto btnRight = new XlsxButton("Sınıf Karma Listeleri", pathClassLists);
        ui->frmLeft->layout()->addWidget(btnLeft);
        ui->frmRight->layout()->addWidget(btnRight);

        if (isHistorical) {
            ui->frmHeader->deleteLater();
            ui->frmVSpacer->layout()->removeItem(ui->verticalSpacer);
            ui->frmForm->layout()->setContentsMargins(0, 0, 0, 0);
            ui->frmRight->layout()->setContentsMargins(0, 0, 0, 0);
            ui->frmLeft->layout()->setContentsMargins(0, 0, 0, 0);
            ui->frame->layout()->setContentsMargins(0, 0, 0, 0);
            layout()->setContentsMargins(0, 0, 0, 0);
            setMaximumHeight(260);
        }
    }

    NSResultsUi::~NSResultsUi()
    {
        delete ui;
    }

} // namespace UI
} // namespace ikoOSKAR
