#include "nspreviewui.h"
#include "ui_nspreviewui.h"

#define GRAY_BG QString("background-color: #B8B8B8; ")

namespace ikoOSKAR {
namespace UI {

    NSPreviewUi::NSPreviewUi(QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::NSPreviewUi)
    {
        ui->setupUi(this);
        ui->tableWidget->setColumnWidth(2, 5);
        ui->tableWidget->setColumnWidth(5, 5);
        ui->tableWidget->setStyleSheet(GRAY_BG + "gridline-color: #B8B8B8");

        for (int row = 0; row < 6; row++) {
            ui->tableWidget->setCellWidget(row, 2, new QLabel());
            ui->tableWidget->setCellWidget(row, 5, new QLabel());
            ui->tableWidget->cellWidget(row, 2)->setStyleSheet(GRAY_BG);
            ui->tableWidget->cellWidget(row, 5)->setStyleSheet(GRAY_BG);

            for (int col = 0; col < 8; col++) {
                auto lbl = new QLabel();
                lbl->setAlignment(Qt::AlignCenter);
                if (col == 2 || col == 5) {
                    lbl->setStyleSheet(GRAY_BG);
                } else {
                    lbl->setStyleSheet("background-color: #EEEEEE");
                }
                ui->tableWidget->setCellWidget(row, col, lbl);
            }
        }
    }

    NSPreviewUi::~NSPreviewUi()
    {
        delete ui;
    }

} // namespace UI
} // namespace ikoOSKAR
