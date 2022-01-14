#include "mainpage.h"
#include "../DatabaseUi/databaseui.h"
#include "ui_mainpage.h"
#include <QDebug>

namespace ikoOSKAR {
namespace UI {

    MainPage::MainPage(QWidget* parent)
        : QMainWindow(parent)
        , ui(new Ui::MainPage)
    {
        ui->setupUi(this);
        changePage(0, new QWidget());

#ifdef QT_DEBUG
        setWindowTitle(this->windowTitle() + " DEBUG");
#endif
    }

    MainPage::~MainPage()
    {
        delete ui;
    }

    void MainPage::changePage(int index, QWidget* subpage)
    {
        const QString moduleNames[5] = { "Ana Sayfa",
            "Öğrenci İşlemleri",
            "Yeni Sınav Düzeni",
            "Önceki Sınav Düzenleri",
            "İletişim ve Uygulama Bilgileri" };
        const QString moduleDescriptions[5] = { "Ana sayfadasınız",
            "Toplam öğrenci sayısı: x",
            "Kalan deneme hakkınız: x | Sınava katılacak öğrenci sayısı",
            "Toplam yapılan sınav sayısı: x",
            "Lisans bilgisi: demo" };
        QPushButton* buttons[5] = { ui->btnHome, ui->btnDatabase, ui->btnNewScheme, ui->btnHistory, ui->btnHelp };

        ui->lblModuleName->setText(moduleNames[index]);
        ui->lblDescription->setText(moduleDescriptions[index]);

        ui->btnCurrentPage->setIcon(buttons[index]->icon());

        if (ui->mainContainer->itemAt(0) != nullptr)
            ui->mainContainer->itemAt(0)->widget()->deleteLater();

        ui->mainContainer->addWidget(subpage, 0, 0, 1, 1);
        if (subpage != nullptr)
            subpage->show();
    }

    void MainPage::on_btnHome_clicked()
    {
        changePage(0, new QWidget());
    }

    void MainPage::on_btnDatabase_clicked()
    {
        changePage(1, new DatabaseUi());
    }

    void MainPage::on_btnNewScheme_clicked()
    {
        changePage(2, new DatabaseUi());
    }

    void MainPage::on_btnHistory_clicked()
    {
        changePage(3, new QWidget());
    }

    void MainPage::on_btnHelp_clicked()
    {
        changePage(4, new QWidget());
    }

} // namespace UI
} // namespace ikoOSKAR
