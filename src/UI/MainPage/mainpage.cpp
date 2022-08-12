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

    /// TODO remove this
    Common::Module* placeholder;

    MainPage::MainPage(QWidget* parent)
        : QMainWindow(parent)
        , ui(new Ui::MainPage)
    {
        placeholder = WelcomeUi::getInstance();
        ui->setupUi(this);

        moduleNames = new QString[] { "Ana Sayfa",
            "Öğrenci İşlemleri",
            "Yeni Sınav Düzeni",
            "Önceki Sınav Düzenleri",
            "İletişim ve Uygulama Bilgileri" };
        moduleDescriptions = new QString[] { "Ana sayfadasınız",
            "Toplam öğrenci sayısı: ",
            "Kalan deneme hakkınız: x | Sınava katılacak öğrenci sayısı",
            "Toplam yapılan sınav sayısı: x",
            "Lisans bilgisi: demo" };
        buttons = new QPushButton* [] { ui->btnHome, ui->btnDatabase, ui->btnNewScheme, ui->btnHistory, ui->btnHelp };
        modules = new Common::Module* [] { WelcomeUi::getInstance(), DatabaseUi::getInstance(), DatabaseUi::getInstance(), placeholder, placeholder };

        for (int i = 0; i < 5; i++) {
            modules[i]->setDescriptionLabel(ui->lblDescription);
            ui->stackedWidget->addWidget(modules[i]);
        }

        changePage(HOME);

#ifdef QT_DEBUG
        setWindowTitle(windowTitle() + " DEBUG");
#endif
    }

    MainPage::~MainPage()
    {
        delete placeholder;
        delete[] moduleNames;
        delete[] moduleDescriptions;
        delete[] buttons;
        delete[] modules;
        delete ui;
    }

    void MainPage::changePage(Subpage index)
    {
        Common::Module* module = modules[index];

        ui->lblModuleName->setText(*module->getName());
        ui->lblDescription->setText(*module->getDescription());
        ui->btnCurrentPage->setIcon(buttons[index]->icon());
        ui->stackedWidget->setCurrentWidget(modules[index]);
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
