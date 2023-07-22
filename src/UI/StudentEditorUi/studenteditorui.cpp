#include "studenteditorui.h"
#include "BLL/StudentEditor/studenteditor.h"
#include "UI/ErrorUi/errorui.h"
#include "ui_studenteditorui.h"

namespace ikoOSKAR {
namespace UI {

    StudentEditorUi::StudentEditorUi(EditorMode mode, QWidget* parent, Shared::Student* student)
        : QDialog(parent)
        , ui(new Ui::StudentEditorUi)
        , student(student)
        , mode(mode)
    {
        ui->setupUi(this);

        switch (mode) {
        case ADD:
            ui->lblHeader->setText("Yeni Öğrenci Ekle");
            setWindowTitle("Yeni Öğrenci Ekle");
            break;
        case EDIT:
            ui->lblHeader->setText("Öğrenci Bilgilerini Düzenle");
            setWindowTitle("Öğrenci Bilgilerini Düzenle");
            ui->spnId->setValue(student->id);
            ui->lnFirstName->setText(student->firstName);
            ui->lnLastName->setText(student->lastName);
            ui->spnGrade->setValue(student->grade);
            ui->cmbSection->setCurrentIndex((int)student->section[0].unicode() - 65);
            break;
        }
    }

    StudentEditorUi::~StudentEditorUi()
    {
        delete ui;
    }

    void StudentEditorUi::done(int r)
    {
        // Reject and return if the cancel button was clicked
        if (QDialog::Rejected == r) {
            QDialog::done(r);
            return;
        }

        // Proceed if the clicked button was the save button
        QString prefix = mode == ADD ? "Öğrenci Eklemede " : "Öğrenci Bilgisi Düzenlemede ";
        QString* errorTitle = new QString(prefix + "Hata Oluştu");
        ErrorUi* errorUi = new ErrorUi(*errorTitle);
        BLL::StudentEditor bll(errorUi);

        int id = ui->spnId->value();
        QString firstName = ui->lnFirstName->text();
        QString lastName = ui->lnLastName->text();
        int grade = ui->spnGrade->value();
        QString section = ui->cmbSection->currentText();

        bool success = false;
        if (mode == ADD) {
            success = bll.createStudent(id, firstName, lastName, grade, section);
        } else if (mode == EDIT) {
            success = bll.updateStudent(*student, id, firstName, lastName, grade, section);
        }

        if (success) {
            QString text = mode == ADD ? "Yeni öğrenci ekleme " : "Öğrenci bilgilerini düzenleme ";
            QMessageBox::information(this, "İşlem Başarılı", text + "işlemi başarılı.");
            QDialog::done(r);
        }
    }

} // namespace UI
} // namespace ikoOSKAR
