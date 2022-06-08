#include "mainpage.h"
#include "ui_mainpage.h"
#include <QDebug>

namespace ikoOSKAR {
namespace UI {
    enum MainPage::Subpage : int {
        HOME,
        DATABASE,
        NEW_SCHEME,
        HISTORY,
        HELP
    };

    // TODO remove this
    QWidget* placeholder;

    MainPage::MainPage(QWidget* parent)
        : QMainWindow(parent)
        , ui(new Ui::MainPage)
    {
        placeholder = new QWidget();
        ui->setupUi(this);
        ui->stackedWidget->addWidget(WelcomeUi::getInstance());
        ui->stackedWidget->addWidget(DatabaseUi::getInstance());
        ui->stackedWidget->addWidget(placeholder);
        ui->stackedWidget->addWidget(placeholder);
        ui->stackedWidget->addWidget(placeholder);

        moduleNames = new QString[] { "Ana Sayfa",
            "Öğrenci İşlemleri",
            "Yeni Sınav Düzeni",
            "Önceki Sınav Düzenleri",
            "İletişim ve Uygulama Bilgileri" };
        moduleDescriptions = new QString[] { "Ana sayfadasınız",
            "Toplam öğrenci sayısı: x",
            "Kalan deneme hakkınız: x | Sınava katılacak öğrenci sayısı",
            "Toplam yapılan sınav sayısı: x",
            "Lisans bilgisi: demo" };
        buttons = new QPushButton* [] { ui->btnHome, ui->btnDatabase, ui->btnNewScheme, ui->btnHistory, ui->btnHelp };
        pages = new QWidget* [] { WelcomeUi::getInstance(), DatabaseUi::getInstance(), DatabaseUi::getInstance(), placeholder, placeholder };

        changePage(HOME);

#ifdef QT_DEBUG
        setWindowTitle(windowTitle() + " DEBUG");
#endif
    }

    MainPage::~MainPage()
    {
        delete ui;
    }

    void MainPage::changePage(Subpage index)
    {
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
