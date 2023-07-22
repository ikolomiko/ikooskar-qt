#include "databaseui.h"
#include "UI/MultiImportUi/multiimportui.h"
#include "UI/StudentEditorUi/studenteditorui.h"
#include "qheaderview.h"
#include "ui_databaseui.h"

#include "UI/ErrorUi/errorui.h"
#include <QLabel>
#include <QMenu>
#include <QTableWidget>

namespace ikoOSKAR {
namespace UI {
    DatabaseUi::DatabaseUi(QWidget* parent)
        : Common::Module(parent)
        , ui(new Ui::DatabaseUi)
    {
        name = new QString("Öğrenci İşlemleri");
        ui->setupUi(this);

        const QString* errorTitle = new QString("Öğrenci İşlemlerinde Hata Oluştu");
        auto error = new ErrorUi(*errorTitle);
        bll = BLL::DatabaseHelper::getInstance(error);

        createButtonMenus();
        createTabWidget();
    }

    DatabaseUi::~DatabaseUi()
    {
        delete ui;
        delete bll;
    }

    const QString* DatabaseUi::getDescription()
    {
        int nStudents = bll->GetNumberOfStudents();
        QString desc("Okul mevcudu: " + QString::number(nStudents));
        if (nStudents == 0)
            return new QString(desc);

        QString currentClass = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
        int classPopulation = bll->GetStudentsByClassName(currentClass)->count();
        return new QString(desc + "  ♦  " + currentClass + " sınıf mevcudu: " + QString::number(classPopulation));
    }

    DatabaseUi* DatabaseUi::getInstance()
    {
        if (instance == nullptr) {
            instance = new DatabaseUi();
        }
        return instance;
    }

    void DatabaseUi::actionAddSingle_clicked()
    {
        StudentEditorUi dialog(StudentEditorUi::ADD, this);
        if (dialog.exec() != QDialog::Rejected) {
            refresh();
        }
    }

    void DatabaseUi::actionAddMulti_clicked()
    {
        MultiImportUi dialog(this);
        if (dialog.exec() != QDialog::Rejected) {
            refresh();
        }
    }

    void DatabaseUi::on_btnEdit_clicked()
    {
        auto* tableWidget = (QTableWidget*)ui->tabWidget->currentWidget();
        if (tableWidget == nullptr) {
            QMessageBox::information(this, "Öğrenci Veri Tabanı Boş", "Hata: Öğrenci veri tabanı boş. Düzenlenecek öğrenci bulunamadı!");
            return;
        }
        int selectedId = tableWidget->selectedItems().at(0)->text().toInt();
        auto* student = bll->GetStudentById(selectedId);
        StudentEditorUi dialog(StudentEditorUi::EDIT, this, student);
        if (dialog.exec() != QDialog::Rejected) {
            refresh();
        }
    }

    void DatabaseUi::on_btnDelete_clicked()
    {
        auto* tableWidget = (QTableWidget*)ui->tabWidget->currentWidget();
        if (tableWidget == nullptr) {
            QMessageBox::information(this, "Öğrenci Veri Tabanı Boş", "Hata: Öğrenci veri tabanı boş. Silinecek öğrenci bulunamadı!");
            return;
        }
        int selectedId = tableWidget->selectedItems().at(0)->text().toInt();
        auto& s = *bll->GetStudentById(selectedId);
        QString text = QString("Adı %1,\nSoyadı %2,\nSınıfı %3,\nŞubesi %4 olan\n%5 numaralı "
                               "öğrenciyi silmek istediğinizden emin misiniz?\n\n BU İŞLEM GERİ ALINAMAZ!")
                           .arg(s.firstName, s.lastName, QString::number(s.grade), s.section, QString::number(s.id));

        auto result = QMessageBox::warning(this, "Silme Onayı", text, QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
        if (result == QMessageBox::Ok) {
            bll->Delete(selectedId);
            refresh();
        }
    }

    void DatabaseUi::actionEoty_clicked()
    {
        QString text = "Yıl sonu işlemleri kapsamında bütün öğrenciler bir sonraki sınıfa aktarılacak, "
                       "12. sınıflar ise veri tabanından SİLİNECEKTİR.\nTamam düğmesine tıkladığınız an "
                       "işlem gerçekleştirilecek ve geri dönüşü olmayacaktır.\nEmin misiniz?";
        auto result = QMessageBox::warning(this, "Yıl Sonu İşlemleri", text, QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
        if (result == QMessageBox::Ok) {
            bll->EndOfTheYear();
            refresh();
        }
    }

    void DatabaseUi::actionRemoveClass_clicked()
    {
        QString currentClass = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
        QString text = currentClass + " sınıfındaki bütün öğrenciler veri tabanından silinecektir. "
                                      "Tamam düğmesine tıkladığınız an işlem gerçekleştirilecek ve geri dönüşü olmayacaktır.\n"
                                      "Emin misiniz?";
        auto result = QMessageBox::warning(this, "Bu Sınıftaki Bütün Öğrencileri Sil", text, QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
        if (result == QMessageBox::Ok) {
            bll->DeleteEntireClass(currentClass);
            refresh();
        }
    }

    void DatabaseUi::createButtonMenus()
    {
        QIcon icon(":/option-bullet.png");

        QMenu* menuAdd = new QMenu();
        QAction* addSingle = new QAction("Öğrenci Bilgilerini Elle Gir");
        addSingle->setIcon(icon);
        connect(addSingle, &QAction::triggered, this, &DatabaseUi::actionAddSingle_clicked);

        QAction* addMulti = new QAction("Öğrenci Bilgilerini Excel Dosyasından Çek");
        addMulti->setIcon(icon);
        connect(addMulti, &QAction::triggered, this, &DatabaseUi::actionAddMulti_clicked);

        menuAdd->addAction(addSingle);
        menuAdd->addAction(addMulti);
        ui->btnAdd->setMenu(menuAdd);

        QMenu* menuMore = new QMenu();
        QAction* eotyAction = new QAction("Yıl sonu işlemlerini yap");
        eotyAction->setIcon(icon);
        connect(eotyAction, &QAction::triggered, this, &DatabaseUi::actionEoty_clicked);

        QAction* removeClassAction = new QAction("Bu sınıftaki bütün öğrencileri sil");
        removeClassAction->setIcon(icon);
        connect(removeClassAction, &QAction::triggered, this, &DatabaseUi::actionRemoveClass_clicked);

        menuMore->addAction(eotyAction);
        menuMore->addAction(removeClassAction);
        ui->btnMore->setMenu(menuMore);
    }

    DatabaseUi::ClassTable::ClassTable(QList<Student*>* students)
    {
        setEditTriggers(QAbstractItemView::NoEditTriggers);
        setAlternatingRowColors(false);
        setVerticalScrollMode(QTableWidget::ScrollPerPixel);
        setHorizontalScrollMode(QTableWidget::ScrollPerPixel);
        setSelectionMode(QAbstractItemView::SingleSelection);
        setSelectionBehavior(QAbstractItemView::SelectRows);
        setShowGrid(true);
        setGridStyle(Qt::SolidLine);
        setCornerButtonEnabled(false);
        setRowCount(students->count());
        setColumnCount(3);
        horizontalHeader()->setStretchLastSection(true);
        verticalHeader()->setVisible(false);
        horizontalHeader()->setMinimumSectionSize(150);
        setHorizontalHeaderLabels({ "Okul No ", "Adı", "Soyadı" });

        for (int i = 0; i < rowCount(); i++) {
            auto* student = students->at(i);
            auto* idItem = new QTableWidgetItem();
            idItem->setData(Qt::DisplayRole, student->id);
            setItem(i, 0, idItem);
            setItem(i, 1, new QTableWidgetItem(student->firstName, 0));
            setItem(i, 2, new QTableWidgetItem(student->lastName, 0));
        }

        setSortingEnabled(true);
        sortItems(0, Qt::AscendingOrder);
        horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        if (students->count() > 0) {
            selectRow(0);
        }
    }

    void DatabaseUi::createTabWidget()
    {
        auto* classNames = bll->GetClassNames();
        auto* tabWidget = ui->tabWidget;

        // Temporarily disable the refresh signal
        disconnect(tabWidget, &QTabWidget::currentChanged, nullptr, nullptr);

        tabWidget->clear();

        for (int i = 0; i < classNames->count(); i++) {
            const auto& className = classNames->value(i);
            auto students = bll->GetStudentsByClassName(className);
            QWidget* tableWidget = new ClassTable(students);
            tabWidget->insertTab(i, tableWidget, className);
        }

        ui->btnDelete->setDisabled(classNames->empty());
        ui->btnEdit->setDisabled(classNames->empty());
        ui->btnMore->setDisabled(classNames->empty());

        // Enable the signal back
        connect(tabWidget, &QTabWidget::currentChanged, this, [&]() {
            lblDescription->setText(*getDescription());
        });
    }

    void DatabaseUi::refresh()
    {
        int index = ui->tabWidget->currentIndex();
        createTabWidget();
        ui->tabWidget->setCurrentIndex(index);
        lblDescription->setText(*getDescription());
    }

} // namespace UI
} // namespace ikoOSKAR
