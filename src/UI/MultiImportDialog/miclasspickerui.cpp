#include "miclasspickerui.h"
#include "ui_miclasspickerui.h"

namespace ikoOSKAR {
namespace UI {

    MIClassPickerUi::MIClassPickerUi(int nParsedStudents, QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::MIClassPickerUi)
    {
        ui->setupUi(this);
        QString placeholderTxt = ui->lblDescription->text();
        placeholderTxt.replace("<n>", QString::number(nParsedStudents));
        ui->lblDescription->setText(placeholderTxt);

        btnPrev = ui->btnPrev;
    }

    MIClassPickerUi::~MIClassPickerUi()
    {
        delete ui;
    }

    void MIClassPickerUi::on_btnNext_clicked()
    {
        emit selectedGradeAndSection(ui->cmbGrade->currentIndex() + 9, ui->cmbSection->currentText());
    }

} // namespace UI
} // namespace ikoOSKAR
