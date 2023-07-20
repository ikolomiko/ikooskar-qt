#include "multiimportui.h"
#include "UI/MultiImportUi/mifilepickerui.h"
#include "ui_multiimportui.h"
#include <QDir>
#include <QFileDialog>
#include <QStandardPaths>
#include <QThreadPool>

namespace ikoOSKAR {
namespace UI {

    MultiImportUi::MultiImportUi(QWidget* parent)
        : QDialog(parent)
        , ui(new Ui::MultiImportUi)
    {
        ui->setupUi(this);
        auto fp = new MIFilePickerUi();
        connect(fp->btnOpenFile, &QPushButton::clicked, this, &MultiImportUi::btnOpenFile_clicked);
        connect(fp->btnHelpExcel, &QPushButton::clicked, this, &MultiImportUi::btnHelpExcel_clicked);
        ui->root->addWidget(fp);
    }

    MultiImportUi::~MultiImportUi()
    {
        delete ui;
    }

    // TODO remove this function
    void test(QList<Shared::Student*>* parsedStudents)
    {
        if (parsedStudents == nullptr || parsedStudents->isEmpty()) {
            return;
        }
        QString txt;
        int i = 0;

        for (const auto& s : *parsedStudents) {
            ++i;
            txt += QString::number(i) + ": " + s->firstName + "-" + s->lastName + "-" + QString::number(s->id) + "\n";
        }

        QMessageBox::information(nullptr, "Success", QString::number(i) + " öğrenci bulundu\n" + txt);
    }

    void MultiImportUi::showSpinner()
    {
        // TODO this method is a horrible hack, gonna implement a proper multipage navigation later
        spinner = new Common::Spinner(this);
        spinner->setTitle("Excel Dosyası Okunuyor");
        for (const auto& ch : this->children()) {
            if (ch->inherits("QWidget")) {
                ((QWidget*)ch)->hide();
            }
        }
        spinner->start();
        spinner->show();
    }

    void MultiImportUi::hideSpinner()
    {
        // TODO this method is a horrible hack, gonna implement a proper multipage navigation later
        for (const auto& ch : this->children()) {
            if (ch->inherits("QWidget")) {
                ((QWidget*)ch)->show();
            }
        }
        spinner->hide();
        spinner->deleteLater();
    }

    const QString* MultiImportUi::pickXlsFile()
    {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.setNameFilter("E-Okul Excel - Sadece Veri (*.XLS *.xls)");
        dialog.setViewMode(QFileDialog::Detail);
        dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));

        if (dialog.exec() == QDialog::Rejected) {
            return nullptr;
        }

        return new QString(dialog.selectedFiles().at(0));
    }

    void MultiImportUi::btnHelpExcel_clicked()
    {
        // TODO show relevant help
    }

    void MultiImportUi::btnOpenFile_clicked()
    {
        auto xlsFilePath = pickXlsFile();
        if (xlsFilePath == nullptr) {
            return;
        }

        // Show the spinner
        showSpinner();

        // Set up the xls parser
        const QString* errorTitle = new QString("Excel'den Öğrenci Eklerken Hata Oluştu");
        auto error = new ErrorUi(*errorTitle);
        auto parser = new BLL::MultiImportHelper(error, *xlsFilePath);

        // Parse the xls file in another thread
        connect(parser, &BLL::MultiImportHelper::parsingFinished, this, &MultiImportUi::handleParsedXls);
        QThreadPool::globalInstance()->start(parser);
    }

    void MultiImportUi::handleParsedXls(QList<Shared::Student*>* parsedStudents)
    {
        hideSpinner();

        // All necessary error messages have been shown already,
        // just return without any further action in case of an error
        if (parsedStudents == nullptr) {
            return;
        }

        test(parsedStudents);

        // All checks passed. Move to the next page and ask for class/grade.
        // Show the number of scanned students as well.
        // Previous and Next buttons at the bottom corners

        // On the next page, display the parsed students in a qtablewidget, much like the one
        // on DatabaseUi. Ask for confirmation.
        // Previous and Confirm buttons at the bottom corners.

        // on_btnconfirm_click
        // for each student set classname and section
        // add them all to the databse
        // msgbox successfully imported class of 9-a with its 10 students
        // return qdialog::accepted
    }

} // namespace UI
} // namespace ikoOSKAR
