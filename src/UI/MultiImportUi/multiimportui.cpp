#include "multiimportui.h"
#include "ui_multiimportui.h"
#include <QDir>
#include <QFileDialog>
#include <QStandardPaths>

namespace ikoOSKAR {
namespace UI {

    MultiImportUi::MultiImportUi(QWidget* parent)
        : QDialog(parent)
        , ui(new Ui::MultiImportUi)
    {
        ui->setupUi(this);

        const QString* errorTitle = new QString("Excel'den Öğrenci Eklerken Hata Oluştu");
        auto error = new ErrorUi(*errorTitle);
        bll = new BLL::MultiImportHelper(error);
    }

    MultiImportUi::~MultiImportUi()
    {
        delete this->bll;
        delete ui;
    }

    void MultiImportUi::on_btnOpenFile_clicked()
    {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.setNameFilter("E-Okul Excel - Sadece Veri (*.XLS *.xls)");
        dialog.setViewMode(QFileDialog::Detail);
        dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));

        if (dialog.exec() == QDialog::Rejected) {
            return;
        }

        QString xlsFilePath = dialog.selectedFiles().at(0);

        // TODO show loading spinner

        auto parsedStudents = bll->parseXls(xlsFilePath);

        // TODO hide loading spinner

        // All necessary error messages have been shown already,
        // just return without any further action in case of an error
        if (parsedStudents == nullptr) {
            return;
        }

        { // Testing
            auto s = parsedStudents->at(0);
            auto inf = s->firstName + "+" + s->lastName + "-" + QString::number(s->id);
            QMessageBox::information(this, "Success", QString::number(parsedStudents->size()) + " öğrenci bulundu\n" + inf);

            auto s2 = parsedStudents->at(parsedStudents->size() - 1);
            auto inf2 = s2->firstName + "+" + s2->lastName + "-" + QString::number(s2->id);
            QMessageBox::information(this, "Success", QString::number(parsedStudents->size()) + " öğrenci bulundu\n" + inf2);
        }

        // All checks passed. Show another popup window and ask for class/grade.
        // Or change the root widget instead of a popup window
        // Show the number of scanned students as well.

        // if dialog.exec == accepted // on_btnsave_click
        // for each student set classname and section
        // add them all to the databse
        // msgbox successfully imported class of 9-a with its 10 students
        // return qdialog::accepted
    }

} // namespace UI
} // namespace ikoOSKAR
