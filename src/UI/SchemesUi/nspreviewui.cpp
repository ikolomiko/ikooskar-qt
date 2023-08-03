#include "nspreviewui.h"
#include "ui_nspreviewui.h"

#define GRAY_BG QString("background-color: #B8B8B8; ")

namespace ikoOSKAR {
namespace UI {

    NSPreviewUi::NSPreviewUi(Preview preview, QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::NSPreviewUi)
    {
        ui->setupUi(this);

        // Exam info label
        QString txt = ui->lblExamInfo->text();
        txt = txt.replace("<x>", preview.examName);
        txt = txt.replace("<y>", preview.examDate);
        ui->lblExamInfo->setText(txt);

        // Attending classes label (header)
        txt = ui->lblClassHeader->text();
        txt = txt.replace("<x>", QString::number(preview.attendingClassNames.count()));
        ui->lblClassHeader->setText(txt);

        // Attending classes label (list)
        txt = preview.attendingClassNames.join(", ");
        ui->lblClasses->setText(txt);

        // Exam halls label (header)
        txt = ui->lblHallHeader->text();
        txt = txt.replace("<x>", QString::number(preview.examHallNames.count()));
        ui->lblHallHeader->setText(txt);

        // Exam halls label (list)
        txt = preview.examHallNames.join(", ");
        ui->lblHalls->setText(txt);

        // General info label
        txt = ui->lblGeneralInfo->text();
        txt = txt.replace("<x>", QString::number(preview.nGrades));
        txt = txt.replace("<y>", QString::number(preview.nStudents));
        txt = txt.replace("<z>", QString::number(preview.totalCapacity));
        ui->lblGeneralInfo->setText(txt);

        // TableWidget settings
        ui->tableWidget->setColumnWidth(2, 5);
        ui->tableWidget->setColumnWidth(5, 5);
        ui->tableWidget->setStyleSheet(GRAY_BG + "gridline-color: #B8B8B8");

        // TableWidget cells
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
                    int effectiveCol = col;

                    if (col > 2 && col < 5)
                        effectiveCol -= 1;
                    if (col > 5)
                        effectiveCol -= 2;

                    int grade = preview.pattern->gradeAt(row, effectiveCol);
                    lbl->setText(QString::number(grade));
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
