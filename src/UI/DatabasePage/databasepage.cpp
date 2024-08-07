#include "databasepage.h"
#include "UI/MultiImportDialog/multiimportdialog.h"
#include "UI/StudentEditorDialog/studenteditordialog.h"
#include "qheaderview.h"
#include "ui_databasepage.h"

#include "UI/ErrorUi/errorui.h"
#include <QLabel>
#include <QMenu>
#include <QTableWidget>

namespace ikoOSKAR {
namespace UI {
    DatabasePage::DatabasePage(QWidget* parent)
        : Common::Page(parent)
        , ui(new Ui::DatabasePage)
    {
        name = new QString("Öğrenci İşlemleri");
        ui->setupUi(this);

        bll = BLL::DatabaseHelper::getInstance();
        connect(bll, &BLL::DatabaseHelper::error, this, &DatabasePage::handleError);

        createButtonMenus();
        createTabWidget();        
    }

    DatabasePage::~DatabasePage()
    {
        delete ui;
        delete bll;
    }

    const QString* DatabasePage::getDescription()
    {
        int nStudents = bll->GetNumberOfStudents();
        QString desc("Okul mevcudu: " + QString::number(nStudents));
        if (nStudents == 0)
            return new QString(desc);

        QString currentClass = currentClassname();
        int classPopulation = bll->GetStudentsByClassName(currentClass)->count();
        return new QString(desc + "  ♦  " + currentClass + " sınıf mevcudu: " + QString::number(classPopulation));
    }

    DatabasePage* DatabasePage::getInstance()
    {
        if (instance == nullptr) {
            instance = new DatabasePage();
        }
        return instance;
    }

    void DatabasePage::actionAddSingle_clicked()
    {
        auto classAndSection = bll->ParseClassName(currentClassname());
        Student* dummyStudent = new Student();
        dummyStudent->grade = classAndSection.first;
        dummyStudent->section = classAndSection.second;

        StudentEditorDialog dialog(StudentEditorDialog::ADD, this, dummyStudent);
        if (dialog.exec() != QDialog::Rejected) {
            refresh();
        }
    }

    void DatabasePage::actionAddMulti_clicked()
    {
        MultiImportDialog dialog(this);
        if (dialog.exec() != QDialog::Rejected) {
            refresh();
        }
    }

    void DatabasePage::on_btnEdit_clicked()
    {
        auto* tableWidget = (QTableWidget*)ui->tabWidget->currentWidget();
        if (tableWidget == nullptr) {
            QMessageBox::information(this, "Öğrenci Veri Tabanı Boş", "Hata: Öğrenci veri tabanı boş. Düzenlenecek öğrenci bulunamadı!");
            return;
        }
        int selectedId = tableWidget->selectedItems().at(0)->text().toInt();
        auto* student = bll->GetStudentById(selectedId);
        StudentEditorDialog dialog(StudentEditorDialog::EDIT, this, student);
        if (dialog.exec() != QDialog::Rejected) {
            refresh();
        }
    }

    void DatabasePage::on_btnDelete_clicked()
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

    void DatabasePage::actionEoty_clicked()
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

    void DatabasePage::actionRemoveClass_clicked()
    {
        QString currentClass = currentClassname();
        QString text = currentClass + " sınıfındaki bütün öğrenciler veri tabanından silinecektir. "
                                      "Tamam düğmesine tıkladığınız an işlem gerçekleştirilecek ve geri dönüşü olmayacaktır.\n"
                                      "Emin misiniz?";
        auto result = QMessageBox::warning(this, "Bu Sınıftaki Bütün Öğrencileri Sil", text, QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
        if (result == QMessageBox::Ok) {
            bll->DeleteEntireClass(currentClass);
            refresh();
        }
    }

    void DatabasePage::handleError(const QString& errorMessage)
    {
        const QString errorTitle = "Öğrenci İşlemlerinde Hata Oluştu";
        ErrorUi(errorTitle).displayMessage(errorMessage);
    }

    void DatabasePage::createButtonMenus()
    {
        QIcon icon(":/option-bullet.png");

        QMenu* menuAdd = new QMenu();
        QAction* addSingle = new QAction("Öğrenci Bilgilerini Elle Gir");
        addSingle->setIcon(icon);
        connect(addSingle, &QAction::triggered, this, &DatabasePage::actionAddSingle_clicked);

        QAction* addMulti = new QAction("Öğrenci Bilgilerini Excel Dosyasından Çek");
        addMulti->setIcon(icon);
        connect(addMulti, &QAction::triggered, this, &DatabasePage::actionAddMulti_clicked);

        menuAdd->addAction(addSingle);
        menuAdd->addAction(addMulti);

        //ui->btnAdd->setMenu(menuAdd);
        connect(ui->btnAdd, &QPushButton::pressed, this, [=](){
            ui->btnAdd->setDown(true);
            QPoint popupPos = ui->frmButtons->mapToGlobal(ui->btnAdd->geometry().bottomLeft());
            menuAdd->exec(popupPos);
        });
        connect(menuAdd, &QMenu::aboutToHide, this, [&]() {
            ui->btnAdd->setDown(false);
        });

        QMenu* menuMore = new QMenu();
        QAction* eotyAction = new QAction("Yıl sonu işlemlerini yap");
        eotyAction->setIcon(icon);
        connect(eotyAction, &QAction::triggered, this, &DatabasePage::actionEoty_clicked);

        QAction* removeClassAction = new QAction("Bu sınıftaki bütün öğrencileri sil");
        removeClassAction->setIcon(icon);
        connect(removeClassAction, &QAction::triggered, this, &DatabasePage::actionRemoveClass_clicked);

        menuMore->addAction(eotyAction);
        menuMore->addAction(removeClassAction);
        //ui->btnMore->setMenu(menuMore);

        connect(ui->btnMore, &QPushButton::pressed, this, [=](){
            ui->btnMore->setDown(true);
            QPoint popupPos = ui->frmButtons->mapToGlobal(ui->btnMore->geometry().bottomLeft());
            menuMore->exec(popupPos);
        });
        connect(menuMore, &QMenu::aboutToHide, this, [&]() {
            ui->btnMore->setDown(false);
        });
    }

    DatabasePage::ClassTable::ClassTable(QList<Student*>* students)
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

    void DatabasePage::createTabWidget()
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
            tableWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
            connect(tableWidget, &QWidget::customContextMenuRequested, this, &DatabasePage::createStudentContextMenu);
        }

        ui->btnDelete->setVisible(!classNames->empty());
        ui->btnEdit->setVisible(!classNames->empty());
        ui->btnMore->setVisible(!classNames->empty());

        // Enable the signal back
        connect(tabWidget, &QTabWidget::currentChanged, this, [&]() {
            emit descriptionUpdated(*getDescription());
        });
    }

    void ikoOSKAR::UI::DatabasePage::createStudentContextMenu()
    {
        QMenu menuStudent;

        QAction edit("Öğrenci Bilgilerini Düzenle");
        edit.setIcon(QIcon(":/edit.png"));
        connect(&edit, &QAction::triggered, this, &DatabasePage::on_btnEdit_clicked);
        menuStudent.addAction(&edit);

        QAction del("Öğrenciyi Sil");
        del.setIcon(QIcon(":/remove.png"));
        connect(&del, &QAction::triggered, this, &DatabasePage::on_btnDelete_clicked);
        menuStudent.addAction(&del);

        menuStudent.exec(QCursor::pos());
    }

    QString DatabasePage::currentClassname()
    {
        int idx = ui->tabWidget->currentIndex();
        QString currentClass = ui->tabWidget->tabText(idx).remove("&");
        return currentClass;
    }

    void DatabasePage::refresh()
    {
        int index = ui->tabWidget->currentIndex();
        createTabWidget();
        ui->tabWidget->setCurrentIndex(index);
        emit descriptionUpdated(*getDescription());
    }

} // namespace UI
} // namespace ikoOSKAR
