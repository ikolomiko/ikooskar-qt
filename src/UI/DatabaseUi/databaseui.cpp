#include "databaseui.h"
#include "ui_databaseui.h"

#include "UI/ErrorUi/errorui.h"
#include <QLabel>
#include <QMenu>

namespace ikoOSKAR {
namespace UI {
    DatabaseUi::DatabaseUi(QWidget* parent)
        : Common::Module(parent)
        , ui(new Ui::DatabaseUi)
    {
        name = new QString("Öğrenci İşlemleri");
        ui->setupUi(this);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableWidget->resizeColumnsToContents();

        const QString* errorTitle = new QString("Öğrenci İşlemlerinde Hata Oluştu");
        auto error = new ErrorUi(*errorTitle);
        bll = new BLL::DatabaseHelper(error);

        QMenu* menuAdd = new QMenu();
        QAction* addSingle = new QAction("Öğrenci Bilgilerini Elle Gir");
        addSingle->setIconVisibleInMenu(false);
        connect(addSingle, &QAction::triggered, this, &DatabaseUi::actionAddSingle_clicked);

        QAction* addMulti = new QAction("Öğrenci Bilgilerini Excel Dosyasından Çek");
        addSingle->setIconVisibleInMenu(false);
        connect(addMulti, &QAction::triggered, this, &DatabaseUi::actionAddMulti_clicked);

        menuAdd->addAction(addSingle);
        menuAdd->addAction(addMulti);
        ui->btnAdd->setMenu(menuAdd);

        ReloadTableData();
    }

    DatabaseUi::~DatabaseUi()
    {
        delete ui;
        delete bll;
    }

    const QString* DatabaseUi::getDescription()
    {
        int nStudents = bll->GetNumberOfStudents();
        return new QString("Toplam öğrenci sayısı: " + QString::number(nStudents));
    }

    DatabaseUi* DatabaseUi::getInstance()
    {
        if (instance == nullptr) {
            instance = new DatabaseUi();
        }
        return instance;
    }

    void DatabaseUi::on_btnAdd_clicked()
    {
        // Debug code here
        /*
        bll->Add(new Student{
                    .id = 1235324234,
                    .firstName = "iko",
                    .lastName = "ğ",
                    .grade = 12,
                    .section = "D"
                 });
        */

        /*
        bll->Update(new Student{
                        .id =  1235324237,
                        .firstName = "UwU",
                        .lastName = "OwO",
                    });
        */
        /*
        bll->Delete(new Student{.id = 1235324235});
        */

        /*
        QString st = "";
        foreach(int i, bll->GetAllIds()){
            st += QString().number(i) + " ";
        }
        ErrorUi(bll->IdExists(1235324235) ? "true" : "false", st);
        */
    }

    void DatabaseUi::on_btnDelete_clicked()
    {
        auto selectedItem = ui->tableWidget->selectionModel()->selectedIndexes().at(0);
        int selectedId = ui->tableWidget->selectedItems().at(0)->text().toInt();
        auto& s = *bll->GetStudentById(selectedId);
        QString text = QString("Adı %1,\nSoyadı %2,\nSınıfı %3,\nŞubesi %4 olan\n%5 numaralı "
                               "öğrenciyi silmek istediğinizden emin misiniz?\n\n BU İŞLEM GERİ ALINAMAZ!")
                           .arg(s.firstName, s.lastName, QString::number(s.grade), s.section, QString::number(s.id));

        auto result = QMessageBox::question(this, "UYARI", text);
        if (result == QMessageBox::Yes) {
            bll->Delete(selectedId);
            ui->tableWidget->removeRow(selectedItem.row());
            lblDescription->setText(*getDescription());
        }
    }

    void DatabaseUi::actionAddSingle_clicked()
    {
        // TODO open StudentEditorUi in new Student mode
        qDebug() << "Add new student";
    }

    void DatabaseUi::actionAddMulti_clicked()
    {
        // TODO open StudentEditorUi in new Student mode
        qDebug() << "Add new students";
    }

    void DatabaseUi::ReloadTableData()
    {
        auto* students = bll->GetStudentsByClassName("12-D");
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(students->count());

        for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
            Student* student = students->at(i);
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(student->id), 1));
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(student->firstName, 0));
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(student->lastName, 0));
        }

        ui->tableWidget->setSortingEnabled(true);
        ui->tableWidget->sortItems(0, Qt::AscendingOrder);
        if (students->count() > 0) {
            ui->tableWidget->selectRow(0);
        }
    }

} // namespace UI
} // namespace ikoOSKAR
