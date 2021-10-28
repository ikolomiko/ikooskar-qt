#include "mainpage.h"
#include "ui_mainpage.h"
#include "../DatabaseUi/databaseui.h"

namespace ikoOSKAR {
namespace UI {

int widgetIndex = 0;
QWidget* widgets[5] = {};

MainPage::MainPage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    changePage(0);
    //widgets[0] = ui->wdContainer;
    //DatabaseUi db;
    widgets[2] = new QWidget();
    widgets[1] = new DatabaseUi();
    widgets[2]->setStyleSheet("background: blue;");
    QGridLayout *l = new QGridLayout(widgets[2]);
    l->setMargin(0);
    l->addWidget(new DatabaseUi(),0,0,1,1);
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


    for (int i = 0; i < 5; i++){
        if (i == index) continue;
        ui->mainContainer->removeWidget(widgets[index]);
        if(widgets[i] != nullptr)
            widgets[i]->hide();
    }
    ui->mainContainer->addWidget(widgets[index],0,0,1,1);
    if(widgets[index] != nullptr) widgets[index]->show();
}

void MainPage::on_btnHome_clicked()
{
    changePage(0);
    //ui->wdContainer
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
