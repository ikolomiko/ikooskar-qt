#include "multiimportui.h"
#include "BLL/DatabaseHelper/databasehelper.h"
#include "BLL/MultiImportHelper/multiimporthelper.h"
#include "UI/Common/spinner.h"
#include "UI/MultiImportUi/miclasspickerui.h"
#include "UI/MultiImportUi/mifilepickerui.h"
#include "UI/MultiImportUi/mipreviewui.h"
#include "ui_multiimportui.h"
#include <QThreadPool>

namespace ikoOSKAR {
namespace UI {

    MultiImportUi::MultiImportUi(QWidget* parent)
        : QDialog(parent)
        , ui(new Ui::MultiImportUi)
    {
        ui->setupUi(this);
        page = FILE_PICKER;
        parsedStudents = nullptr;

        auto fp = new MIFilePickerUi();
        connect(fp, &MIFilePickerUi::pickedXlsFile, this, &MultiImportUi::handleXlsPath);
        ui->root->addWidget(fp);
    }

    MultiImportUi::~MultiImportUi()
    {
        delete parsedStudents;
        delete ui;
    }

    void MultiImportUi::prevPage()
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

    void MultiImportUi::nextPage()
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
            connect(classPicker->btnPrev, &QPushButton::clicked, this, &MultiImportUi::prevPage);
            connect(classPicker, &MIClassPickerUi::selectedGradeAndSection, this, &MultiImportUi::handleGradeAndSection);
            ui->root->addWidget(classPicker);
            ui->root->setCurrentIndex(ui->root->currentIndex() + 1);
            break;
        }
        case CLASS_PICKER: {
            auto preview = new MIPreviewUi(parsedStudents, this);
            connect(preview->btnPrev, &QPushButton::clicked, this, &MultiImportUi::prevPage);
            connect(preview->btnConfirm, &QPushButton::clicked, this, &MultiImportUi::handleConfirmation);
            ui->root->addWidget(preview);
            ui->root->setCurrentIndex(ui->root->currentIndex() + 1);
            break;
        }
        case PREVIEW:
            // Close the MultiImportUi dialog with the accepted signal
            accept();
            return;
        }

        // Increment the pagestate by one
        page = (PageState)((int)(page) + 1);
    }

    void MultiImportUi::handleXlsPath(QString* xlsFilePath)
    {
        if (xlsFilePath == nullptr) {
            return;
        }

        // Show the spinner
        nextPage(); // FilePicker -> Spinner

        // Set up the xls parser
        const QString* errorTitle = new QString("Excel'den Öğrenci Eklerken Hata Oluştu");
        auto error = new ErrorUi(*errorTitle);
        auto parser = new BLL::MultiImportHelper(error, *xlsFilePath);

        // Parse the xls file in another thread
        connect(parser, &BLL::MultiImportHelper::parsingFinished, this, &MultiImportUi::handleParsedXls);
        QThreadPool::globalInstance()->start(parser);
    }

    void MultiImportUi::handleParsedXls(QList<Student*>* parsedStudents)
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

    void MultiImportUi::handleGradeAndSection(int grade, const QString& section)
    {
        // Apply the selected grade and section for all parsed students
        for (auto student : *parsedStudents) {
            student->grade = grade;
            student->section = section;
        }

        nextPage(); // ClassPicker -> Preview
    }

    void MultiImportUi::handleConfirmation()
    {
        // Add all students to the database
        const QString* errorTitle = new QString("Excel'den Öğrenci Eklerken Hata Oluştu");
        auto db = BLL::DatabaseHelper::getInstance(new ErrorUi(*errorTitle));
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

} // namespace UI
} // namespace ikoOSKAR
