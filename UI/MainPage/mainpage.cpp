#include "mainpage.h"
#include "ui_mainpage.h"

namespace ikoOSKAR {
namespace UI {

int widgetIndex = 0;

MainPage::MainPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    changePage(0);
}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::changePage(int index){
    const QString moduleNames[5] = {"Ana Sayfa",
                                    "Öğrenci İşlemleri",
                                    "Yeni Sınav Düzeni",
                                    "Önceki Sınav Düzenleri",
                                    "İletişim ve Uygulama Bilgileri"};
    const QString moduleDescriptions[5] = {"Ana sayfadasınız",
                                           "Toplam öğrenci sayısı: x",
                                           "Kalan deneme hakkınız: x | Sınava katılacak öğrenci sayısı",
                                           "Toplam yapılan sınav sayısı: x",
                                           "Lisans bilgisi: demo"};
    QPushButton* buttons[5] = {ui->btnHome, ui->btnDatabase, ui->btnNewScheme, ui->btnHistory, ui->btnHelp};

    ui->lblModuleName->setText(moduleNames[index]);
    ui->lblDescription->setText(moduleDescriptions[index]);

    ui->btnCurrentPage->setIcon(buttons[index]->icon());

    /// TODO also change widget
}

void MainPage::on_btnHome_clicked()
{
    changePage(0);
}

void MainPage::on_btnDatabase_clicked()
{
    changePage(1);
}


void MainPage::on_btnNewScheme_clicked()
{
  changePage(2);
}


void MainPage::on_btnHistory_clicked()
{
  changePage(3);
}


void MainPage::on_btnHelp_clicked()
{
  changePage(4);
}

} // namespace UI
} // namespace ikoOSKAR
