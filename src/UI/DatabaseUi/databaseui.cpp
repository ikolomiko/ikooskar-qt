#include "databaseui.h"
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
        connect(ui->tabWidget, &QTabWidget::currentChanged, this, [&]() {
            lblDescription->setText(*getDescription());
        });
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
        dialog.exec();
        refresh();
    }

    void DatabaseUi::actionAddMulti_clicked()
    {
        // TODO open StudentEditorUi in new Student mode
        qDebug() << "Add new students";
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
        dialog.exec();
        refresh();
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

    QTableWidget* DatabaseUi::createClassTable(const QString& className)
    {
        auto* students = bll->GetStudentsByClassName(className);

        auto* table = new QTableWidget();
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setAlternatingRowColors(false);
        table->setVerticalScrollMode(QTableWidget::ScrollPerPixel);
        table->setHorizontalScrollMode(QTableWidget::ScrollPerPixel);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setShowGrid(true);
        table->setGridStyle(Qt::SolidLine);
        table->setCornerButtonEnabled(false);
        table->setRowCount(students->count());
        table->setColumnCount(3);
        table->horizontalHeader()->setStretchLastSection(true);
        table->verticalHeader()->setVisible(false);
        table->horizontalHeader()->setMinimumSectionSize(150);
        table->setHorizontalHeaderLabels(QStringList() << "Okul No "
                                                       << "Adı"
                                                       << "Soyadı");

        for (int i = 0; i < table->rowCount(); i++) {
            Student* student = students->at(i);
            table->setItem(i, 0, new QTableWidgetItem(QString::number(student->id), 1));
            table->setItem(i, 1, new QTableWidgetItem(student->firstName, 0));
            table->setItem(i, 2, new QTableWidgetItem(student->lastName, 0));
        }

        table->setSortingEnabled(true);
        table->sortItems(0, Qt::AscendingOrder);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        if (students->count() > 0) {
            table->selectRow(0);
        }

        return table;
    }

    void DatabaseUi::createTabWidget()
    {
        auto* classNames = bll->GetClassNames();
        auto* tabWidget = ui->tabWidget;

        tabWidget->clear();

        for (int i = 0; i < classNames->count(); i++) {
            const auto& className = classNames->value(i);
            auto* tableWidget = createClassTable(className);
            tabWidget->insertTab(i, tableWidget, className);
        }

        ui->btnDelete->setDisabled(classNames->empty());
        ui->btnEdit->setDisabled(classNames->empty());
        ui->btnMore->setDisabled(classNames->empty());
    }

    void DatabaseUi::refresh()
    {
        createTabWidget();
        lblDescription->setText(*getDescription());
    }

} // namespace UI
} // namespace ikoOSKAR
