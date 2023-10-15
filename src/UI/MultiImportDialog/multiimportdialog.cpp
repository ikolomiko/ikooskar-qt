#include "multiimportdialog.h"
#include "BLL/DatabaseHelper/databasehelper.h"
#include "BLL/MultiImportHelper/multiimporthelper.h"
#include "UI/Common/spinner.h"
#include "UI/ErrorUi/errorui.h"
#include "UI/MultiImportDialog/miclasspickerui.h"
#include "UI/MultiImportDialog/mifilepickerui.h"
#include "UI/MultiImportDialog/mipreviewui.h"
#include "ui_multiimportdialog.h"
#include <QThreadPool>

namespace ikoOSKAR {
namespace UI {

    MultiImportDialog::MultiImportDialog(QWidget* parent)
        : QDialog(parent)
        , ui(new Ui::MultiImportDialog)
    {
        ui->setupUi(this);
        page = FILE_PICKER;
        parsedStudents = nullptr;

        auto fp = new MIFilePickerUi();
        connect(fp, &MIFilePickerUi::pickedXlsFile, this, &MultiImportDialog::handleXlsPath);
        ui->root->addWidget(fp);
    }

    MultiImportDialog::~MultiImportDialog()
    {
        delete parsedStudents;
        delete ui;
    }

    void MultiImportDialog::prevPage()
    {
        if (page == FILE_PICKER) {
            // Illegal state, no way to end up here
            return;
        }

        // For either of the spinner, classpicker and preview widgets, the procedure is the same
        auto currentWidget = ui->root->widget(ui->root->currentIndex());
        ui->root->removeWidget(currentWidget);
        currentWidget->deleteLater();

        // Decrement the pagestate by one
        page = (PageState)((int)(page)-1);

        // Don't let the pagestate stick in spinner state
        if (page == SPINNER) {
            page = FILE_PICKER;
        }
    }

    void MultiImportDialog::nextPage()
    {
        switch (page) {
        case FILE_PICKER: {
            auto spinner = new Common::Spinner(this);
            spinner->setTitle("Excel Dosyası Okunuyor");
            ui->root->addWidget(spinner);
            ui->root->setCurrentIndex(ui->root->currentIndex() + 1);
            spinner->start();
            break;
        }
        case SPINNER: {
            // Don't keep the spinner widget in the page history
            auto spinner = (Common::Spinner*)ui->root->widget(ui->root->currentIndex());
            ui->root->removeWidget(spinner);
            spinner->stop();
            spinner->deleteLater();

            auto classPicker = new MIClassPickerUi(parsedStudents->size(), this);
            connect(classPicker->btnPrev, &QPushButton::clicked, this, &MultiImportDialog::prevPage);
            connect(classPicker, &MIClassPickerUi::selectedGradeAndSection, this, &MultiImportDialog::handleGradeAndSection);
            ui->root->addWidget(classPicker);
            ui->root->setCurrentIndex(ui->root->currentIndex() + 1);
            break;
        }
        case CLASS_PICKER: {
            auto preview = new MIPreviewUi(parsedStudents, this);
            connect(preview->btnPrev, &QPushButton::clicked, this, &MultiImportDialog::prevPage);
            connect(preview->btnConfirm, &QPushButton::clicked, this, &MultiImportDialog::handleConfirmation);
            ui->root->addWidget(preview);
            ui->root->setCurrentIndex(ui->root->currentIndex() + 1);
            break;
        }
        case PREVIEW:
            // Close the MultiImportDialog dialog with the accepted signal
            accept();
            return;
        }

        // Increment the pagestate by one
        page = (PageState)((int)(page) + 1);
    }

    void MultiImportDialog::handleXlsPath(QString* xlsFilePath)
    {
        if (xlsFilePath == nullptr) {
            return;
        }

        // Show the spinner
        nextPage(); // FilePicker -> Spinner

        // Set up the xls parser
        auto parser = new BLL::MultiImportHelper(*xlsFilePath);
        connect(parser, &BLL::MultiImportHelper::error, this, &MultiImportDialog::handleError);

        // Parse the xls file in another thread
        connect(parser, &BLL::MultiImportHelper::parsingFinished, this, &MultiImportDialog::handleParsedXls);
        QThreadPool::globalInstance()->start(parser);
    }

    void MultiImportDialog::handleParsedXls(QList<Student*>* parsedStudents)
    {
        delete this->parsedStudents;
        this->parsedStudents = parsedStudents;

        // All necessary error messages have been shown already,
        // just go to the previous page without displaying any error messages in case of an error
        if (parsedStudents == nullptr) {
            prevPage(); // Spinner -> FilePicker
            return;
        }

        // All checks passed. Move to the next page and ask for class/grade.
        nextPage(); // Spinner -> ClassPicker
    }

    void MultiImportDialog::handleGradeAndSection(int grade, const QString& section)
    {
        // Apply the selected grade and section for all parsed students
        for (auto student : *parsedStudents) {
            student->grade = grade;
            student->section = section;
        }

        nextPage(); // ClassPicker -> Preview
    }

    void MultiImportDialog::handleConfirmation()
    {
        // Add all students to the database
        auto db = BLL::DatabaseHelper::getInstance();
        int populationBefore = db->GetAllIds().size();
        db->AddAll(*parsedStudents);
        int populationDiff = db->GetAllIds().size() - populationBefore;

        if (populationDiff == parsedStudents->size()) {
            // All students were added to the database successfully
            QMessageBox::information(this, "Başarılı", QString::number(populationDiff) + " öğrenci başarıyla veri tabanına eklendi.");
        } else {
            // Some students didn't get added to the database
            // No need to list them, since the proper error message was already shown by the BLL
            QString message = QString("Excel dosyasından okunan %1 öğrenciden %2 öğrenci veri tabanına eklendi, "
                                      "%3 öğrencide hata oluştuğu için veri tabanına eklenemedi.")
                                  .arg(parsedStudents->size(), populationDiff, parsedStudents->size() - populationDiff);
            QMessageBox::information(this, "Kısmen Başarılı", message);
        }

        nextPage(); // Preview -> [dialog closed]
    }

    void MultiImportDialog::handleError(const QString& errorMessage)
    {
        const QString errorTitle = "Excel'den Öğrenci Eklerken Hata Oluştu";
        ErrorUi(errorTitle).displayMessage(errorMessage);
    }

} // namespace UI
} // namespace ikoOSKAR
