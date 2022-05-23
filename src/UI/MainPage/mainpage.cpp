#include "mainpage.h"
#include "../DatabaseUi/databaseui.h"
#include "ui_mainpage.h"
#include <QDebug>

namespace ikoOSKAR {
namespace UI {

    enum MainPage::Subpage : int { HOME,
        DATABASE,
        NEW_SCHEME,
        HISTORY,
        HELP };

    // TODO remove this
    QWidget* placeholder;

    MainPage::MainPage(QWidget* parent)
        : QMainWindow(parent)
        , ui(new Ui::MainPage)
    {
        placeholder = new QWidget();
        ui->setupUi(this);
        ui->stackedWidget->addWidget(placeholder);
        ui->stackedWidget->addWidget(DatabaseUi::getInstance());
        ui->stackedWidget->addWidget(DatabaseUi::getInstance());
        ui->stackedWidget->addWidget(placeholder);
        ui->stackedWidget->addWidget(placeholder);
        changePage(HOME);

#ifdef QT_DEBUG
        setWindowTitle(this->windowTitle() + " DEBUG");
#endif
    }

    MainPage::~MainPage()
    {
        delete ui;
    }

    void MainPage::changePage(Subpage index)
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
        QWidget* pages[5] = { placeholder, DatabaseUi::getInstance(), DatabaseUi::getInstance(), placeholder, placeholder };

        ui->lblModuleName->setText(moduleNames[index]);
        ui->lblDescription->setText(moduleDescriptions[index]);

        ui->btnCurrentPage->setIcon(buttons[index]->icon());

        ui->stackedWidget->setCurrentWidget(pages[index]);
    }

    void MainPage::on_btnHome_clicked()
    {
        changePage(HOME);
    }

    void MainPage::on_btnDatabase_clicked()
    {
        changePage(DATABASE);
    }

    void MainPage::on_btnNewScheme_clicked()
    {
        changePage(NEW_SCHEME);
    }

    void MainPage::on_btnHistory_clicked()
    {
        changePage(HISTORY);
    }

    void MainPage::on_btnHelp_clicked()
    {
        changePage(HELP);
    }

} // namespace UI
} // namespace ikoOSKAR
